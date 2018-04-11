/*
 *
 * Copyright 2017 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <grpc/impl/codegen/byte_buffer.h>
#include <grpc/slice.h>
#include <grpcpp/impl/codegen/grpc_library.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/grpc_library.h>
#include <gtest/gtest.h>

namespace grpc {

namespace internal {

// Provide access to GrpcProtoBufferWriter internals.
class GrpcProtoBufferWriterPeer {
 public:
  explicit GrpcProtoBufferWriterPeer(GrpcProtoBufferWriter* writer)
      : writer_(writer) {}
  bool have_backup() const { return writer_->have_backup_; }
  const grpc_slice& backup_slice() const { return writer_->backup_slice_; }
  const grpc_slice& slice() const { return writer_->slice_; }

 private:
  GrpcProtoBufferWriter* writer_;
};

// Provide access to ByteBuffer internals.
class GrpcByteBufferPeer {
 public:
  explicit GrpcByteBufferPeer(ByteBuffer* bb) : bb_(bb) {}
  grpc_byte_buffer* c_buffer() { return bb_->c_buffer(); }

 private:
  ByteBuffer* bb_;
};

class ProtoUtilsTest : public ::testing::Test {};

// Regression test for a memory corruption bug where a series of
// GrpcProtoBufferWriter Next()/Backup() invocations could result in a dangling
// pointer returned by Next() due to the interaction between grpc_slice inlining
// and GRPC_SLICE_START_PTR.
TEST_F(ProtoUtilsTest, TinyBackupThenNext) {
  ByteBuffer bp;
  const int block_size = 1024;
  GrpcProtoBufferWriter writer(&bp, block_size, 8192);
  GrpcProtoBufferWriterPeer peer(&writer);

  void* data;
  int size;
  // Allocate a slice.
  ASSERT_TRUE(writer.Next(&data, &size));
  EXPECT_EQ(block_size, size);
  // Return a single byte.
  writer.BackUp(1);
  EXPECT_FALSE(peer.have_backup());
  // On the next allocation, the returned slice is non-inlined.
  ASSERT_TRUE(writer.Next(&data, &size));
  EXPECT_TRUE(peer.slice().refcount != nullptr);
  EXPECT_EQ(block_size, size);
}

namespace {

// Set backup_size to 0 to indicate no backup is needed.
void BufferWriterTest(int block_size, int total_size, int backup_size) {
  ByteBuffer bb;
  GrpcProtoBufferWriter writer(&bb, block_size, total_size);

  int written_size = 0;
  void* data;
  int size = 0;
  bool backed_up_entire_slice = false;

  while (written_size < total_size) {
    EXPECT_TRUE(writer.Next(&data, &size));
    EXPECT_GT(size, 0);
    EXPECT_TRUE(data);
    int write_size = size;
    bool should_backup = false;
    if (backup_size > 0 && size > backup_size) {
      write_size = size - backup_size;
      should_backup = true;
    } else if (size == backup_size && !backed_up_entire_slice) {
      // only backup entire slice once.
      backed_up_entire_slice = true;
      should_backup = true;
      write_size = 0;
    }
    // May need a last backup.
    if (write_size + written_size > total_size) {
      write_size = total_size - written_size;
      should_backup = true;
      backup_size = size - write_size;
      ASSERT_GT(backup_size, 0);
    }
    for (int i = 0; i < write_size; i++) {
      (static_cast<uint8_t*>(data))[i] = written_size % 128;
      written_size++;
    }
    if (should_backup) {
      writer.BackUp(backup_size);
    }
  }
  EXPECT_EQ(bb.Length(), (size_t)total_size);

  grpc_byte_buffer_reader reader;
  GrpcByteBufferPeer peer(&bb);
  grpc_byte_buffer_reader_init(&reader, peer.c_buffer());
  int read_bytes = 0;
  while (read_bytes < total_size) {
    grpc_slice s;
    EXPECT_TRUE(grpc_byte_buffer_reader_next(&reader, &s));
    for (size_t i = 0; i < GRPC_SLICE_LENGTH(s); i++) {
      EXPECT_EQ(GRPC_SLICE_START_PTR(s)[i], read_bytes % 128);
      read_bytes++;
    }
    grpc_slice_unref(s);
  }
  EXPECT_EQ(read_bytes, total_size);
  grpc_byte_buffer_reader_destroy(&reader);
}

TEST(WriterTest, TinyBlockTinyBackup) {
  for (int i = 2; i < static_cast<int> GRPC_SLICE_INLINED_SIZE; i++) {
    BufferWriterTest(i, 256, 1);
  }
}

TEST(WriterTest, SmallBlockTinyBackup) { BufferWriterTest(64, 256, 1); }

TEST(WriterTest, SmallBlockNoBackup) { BufferWriterTest(64, 256, 0); }

TEST(WriterTest, SmallBlockFullBackup) { BufferWriterTest(64, 256, 64); }

TEST(WriterTest, LargeBlockTinyBackup) { BufferWriterTest(4096, 8192, 1); }

TEST(WriterTest, LargeBlockNoBackup) { BufferWriterTest(4096, 8192, 0); }

TEST(WriterTest, LargeBlockFullBackup) { BufferWriterTest(4096, 8192, 4096); }

TEST(WriterTest, LargeBlockLargeBackup) { BufferWriterTest(4096, 8192, 4095); }

}  // namespace
}  // namespace internal
}  // namespace grpc

int main(int argc, char** argv) {
  // Ensure the GrpcProtoBufferWriter internals are initialized.
  grpc::internal::GrpcLibraryInitializer init;
  init.summon();
  grpc::GrpcLibraryCodegen lib;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* This file was generated by upbc (the upb compiler) from the input
 * file:
 *
 *     google/protobuf/descriptor.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated. */

#include "google/protobuf/descriptor.upb.h"
#include <stddef.h>
#include "upb/msg.h"

#include "upb/port_def.inc"

static const upb_msglayout* const google_protobuf_FileDescriptorSet_submsgs[1] =
    {
        &google_protobuf_FileDescriptorProto_msginit,
};

static const upb_msglayout_field google_protobuf_FileDescriptorSet__fields[1] =
    {
        {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_FileDescriptorSet_msginit = {
    &google_protobuf_FileDescriptorSet_submsgs[0],
    &google_protobuf_FileDescriptorSet__fields[0],
    UPB_SIZE(4, 8),
    1,
    false,
};

static const upb_msglayout* const
    google_protobuf_FileDescriptorProto_submsgs[6] = {
        &google_protobuf_DescriptorProto_msginit,
        &google_protobuf_EnumDescriptorProto_msginit,
        &google_protobuf_FieldDescriptorProto_msginit,
        &google_protobuf_FileOptions_msginit,
        &google_protobuf_ServiceDescriptorProto_msginit,
        &google_protobuf_SourceCodeInfo_msginit,
};

static const upb_msglayout_field
    google_protobuf_FileDescriptorProto__fields[12] = {
        {1, UPB_SIZE(8, 16), 1, 0, 9, 1},
        {2, UPB_SIZE(16, 32), 2, 0, 9, 1},
        {3, UPB_SIZE(40, 80), 0, 0, 9, 3},
        {4, UPB_SIZE(44, 88), 0, 0, 11, 3},
        {5, UPB_SIZE(48, 96), 0, 1, 11, 3},
        {6, UPB_SIZE(52, 104), 0, 4, 11, 3},
        {7, UPB_SIZE(56, 112), 0, 2, 11, 3},
        {8, UPB_SIZE(32, 64), 4, 3, 11, 1},
        {9, UPB_SIZE(36, 72), 5, 5, 11, 1},
        {10, UPB_SIZE(60, 120), 0, 0, 5, 3},
        {11, UPB_SIZE(64, 128), 0, 0, 5, 3},
        {12, UPB_SIZE(24, 48), 3, 0, 9, 1},
};

const upb_msglayout google_protobuf_FileDescriptorProto_msginit = {
    &google_protobuf_FileDescriptorProto_submsgs[0],
    &google_protobuf_FileDescriptorProto__fields[0],
    UPB_SIZE(72, 144),
    12,
    false,
};

static const upb_msglayout* const google_protobuf_DescriptorProto_submsgs[8] = {
    &google_protobuf_DescriptorProto_msginit,
    &google_protobuf_DescriptorProto_ExtensionRange_msginit,
    &google_protobuf_DescriptorProto_ReservedRange_msginit,
    &google_protobuf_EnumDescriptorProto_msginit,
    &google_protobuf_FieldDescriptorProto_msginit,
    &google_protobuf_MessageOptions_msginit,
    &google_protobuf_OneofDescriptorProto_msginit,
};

static const upb_msglayout_field google_protobuf_DescriptorProto__fields[10] = {
    {1, UPB_SIZE(8, 16), 1, 0, 9, 1},   {2, UPB_SIZE(20, 40), 0, 4, 11, 3},
    {3, UPB_SIZE(24, 48), 0, 0, 11, 3}, {4, UPB_SIZE(28, 56), 0, 3, 11, 3},
    {5, UPB_SIZE(32, 64), 0, 1, 11, 3}, {6, UPB_SIZE(36, 72), 0, 4, 11, 3},
    {7, UPB_SIZE(16, 32), 2, 5, 11, 1}, {8, UPB_SIZE(40, 80), 0, 6, 11, 3},
    {9, UPB_SIZE(44, 88), 0, 2, 11, 3}, {10, UPB_SIZE(48, 96), 0, 0, 9, 3},
};

const upb_msglayout google_protobuf_DescriptorProto_msginit = {
    &google_protobuf_DescriptorProto_submsgs[0],
    &google_protobuf_DescriptorProto__fields[0],
    UPB_SIZE(56, 112),
    10,
    false,
};

static const upb_msglayout* const
    google_protobuf_DescriptorProto_ExtensionRange_submsgs[1] = {
        &google_protobuf_ExtensionRangeOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_DescriptorProto_ExtensionRange__fields[3] = {
        {1, UPB_SIZE(4, 4), 1, 0, 5, 1},
        {2, UPB_SIZE(8, 8), 2, 0, 5, 1},
        {3, UPB_SIZE(12, 16), 3, 0, 11, 1},
};

const upb_msglayout google_protobuf_DescriptorProto_ExtensionRange_msginit = {
    &google_protobuf_DescriptorProto_ExtensionRange_submsgs[0],
    &google_protobuf_DescriptorProto_ExtensionRange__fields[0],
    UPB_SIZE(16, 24),
    3,
    false,
};

static const upb_msglayout_field
    google_protobuf_DescriptorProto_ReservedRange__fields[2] = {
        {1, UPB_SIZE(4, 4), 1, 0, 5, 1},
        {2, UPB_SIZE(8, 8), 2, 0, 5, 1},
};

const upb_msglayout google_protobuf_DescriptorProto_ReservedRange_msginit = {
    NULL,
    &google_protobuf_DescriptorProto_ReservedRange__fields[0],
    UPB_SIZE(12, 12),
    2,
    false,
};

static const upb_msglayout* const
    google_protobuf_ExtensionRangeOptions_submsgs[1] = {
        &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field
    google_protobuf_ExtensionRangeOptions__fields[1] = {
        {999, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_ExtensionRangeOptions_msginit = {
    &google_protobuf_ExtensionRangeOptions_submsgs[0],
    &google_protobuf_ExtensionRangeOptions__fields[0],
    UPB_SIZE(4, 8),
    1,
    false,
};

static const upb_msglayout* const
    google_protobuf_FieldDescriptorProto_submsgs[1] = {
        &google_protobuf_FieldOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_FieldDescriptorProto__fields[10] = {
        {1, UPB_SIZE(32, 32), 5, 0, 9, 1},
        {2, UPB_SIZE(40, 48), 6, 0, 9, 1},
        {3, UPB_SIZE(24, 24), 3, 0, 5, 1},
        {4, UPB_SIZE(8, 8), 1, 0, 14, 1},
        {5, UPB_SIZE(16, 16), 2, 0, 14, 1},
        {6, UPB_SIZE(48, 64), 7, 0, 9, 1},
        {7, UPB_SIZE(56, 80), 8, 0, 9, 1},
        {8, UPB_SIZE(72, 112), 10, 0, 11, 1},
        {9, UPB_SIZE(28, 28), 4, 0, 5, 1},
        {10, UPB_SIZE(64, 96), 9, 0, 9, 1},
};

const upb_msglayout google_protobuf_FieldDescriptorProto_msginit = {
    &google_protobuf_FieldDescriptorProto_submsgs[0],
    &google_protobuf_FieldDescriptorProto__fields[0],
    UPB_SIZE(80, 128),
    10,
    false,
};

static const upb_msglayout* const
    google_protobuf_OneofDescriptorProto_submsgs[1] = {
        &google_protobuf_OneofOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_OneofDescriptorProto__fields[2] = {
        {1, UPB_SIZE(8, 16), 1, 0, 9, 1},
        {2, UPB_SIZE(16, 32), 2, 0, 11, 1},
};

const upb_msglayout google_protobuf_OneofDescriptorProto_msginit = {
    &google_protobuf_OneofDescriptorProto_submsgs[0],
    &google_protobuf_OneofDescriptorProto__fields[0],
    UPB_SIZE(24, 48),
    2,
    false,
};

static const upb_msglayout* const
    google_protobuf_EnumDescriptorProto_submsgs[3] = {
        &google_protobuf_EnumDescriptorProto_EnumReservedRange_msginit,
        &google_protobuf_EnumOptions_msginit,
        &google_protobuf_EnumValueDescriptorProto_msginit,
};

static const upb_msglayout_field
    google_protobuf_EnumDescriptorProto__fields[5] = {
        {1, UPB_SIZE(8, 16), 1, 0, 9, 1},   {2, UPB_SIZE(20, 40), 0, 2, 11, 3},
        {3, UPB_SIZE(16, 32), 2, 1, 11, 1}, {4, UPB_SIZE(24, 48), 0, 0, 11, 3},
        {5, UPB_SIZE(28, 56), 0, 0, 9, 3},
};

const upb_msglayout google_protobuf_EnumDescriptorProto_msginit = {
    &google_protobuf_EnumDescriptorProto_submsgs[0],
    &google_protobuf_EnumDescriptorProto__fields[0],
    UPB_SIZE(32, 64),
    5,
    false,
};

static const upb_msglayout_field
    google_protobuf_EnumDescriptorProto_EnumReservedRange__fields[2] = {
        {1, UPB_SIZE(4, 4), 1, 0, 5, 1},
        {2, UPB_SIZE(8, 8), 2, 0, 5, 1},
};

const upb_msglayout
    google_protobuf_EnumDescriptorProto_EnumReservedRange_msginit = {
        NULL,
        &google_protobuf_EnumDescriptorProto_EnumReservedRange__fields[0],
        UPB_SIZE(12, 12),
        2,
        false,
};

static const upb_msglayout* const
    google_protobuf_EnumValueDescriptorProto_submsgs[1] = {
        &google_protobuf_EnumValueOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_EnumValueDescriptorProto__fields[3] = {
        {1, UPB_SIZE(8, 16), 2, 0, 9, 1},
        {2, UPB_SIZE(4, 4), 1, 0, 5, 1},
        {3, UPB_SIZE(16, 32), 3, 0, 11, 1},
};

const upb_msglayout google_protobuf_EnumValueDescriptorProto_msginit = {
    &google_protobuf_EnumValueDescriptorProto_submsgs[0],
    &google_protobuf_EnumValueDescriptorProto__fields[0],
    UPB_SIZE(24, 48),
    3,
    false,
};

static const upb_msglayout* const
    google_protobuf_ServiceDescriptorProto_submsgs[2] = {
        &google_protobuf_MethodDescriptorProto_msginit,
        &google_protobuf_ServiceOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_ServiceDescriptorProto__fields[3] = {
        {1, UPB_SIZE(8, 16), 1, 0, 9, 1},
        {2, UPB_SIZE(20, 40), 0, 0, 11, 3},
        {3, UPB_SIZE(16, 32), 2, 1, 11, 1},
};

const upb_msglayout google_protobuf_ServiceDescriptorProto_msginit = {
    &google_protobuf_ServiceDescriptorProto_submsgs[0],
    &google_protobuf_ServiceDescriptorProto__fields[0],
    UPB_SIZE(24, 48),
    3,
    false,
};

static const upb_msglayout* const
    google_protobuf_MethodDescriptorProto_submsgs[1] = {
        &google_protobuf_MethodOptions_msginit,
};

static const upb_msglayout_field
    google_protobuf_MethodDescriptorProto__fields[6] = {
        {1, UPB_SIZE(8, 16), 3, 0, 9, 1},  {2, UPB_SIZE(16, 32), 4, 0, 9, 1},
        {3, UPB_SIZE(24, 48), 5, 0, 9, 1}, {4, UPB_SIZE(32, 64), 6, 0, 11, 1},
        {5, UPB_SIZE(1, 1), 1, 0, 8, 1},   {6, UPB_SIZE(2, 2), 2, 0, 8, 1},
};

const upb_msglayout google_protobuf_MethodDescriptorProto_msginit = {
    &google_protobuf_MethodDescriptorProto_submsgs[0],
    &google_protobuf_MethodDescriptorProto__fields[0],
    UPB_SIZE(40, 80),
    6,
    false,
};

static const upb_msglayout* const google_protobuf_FileOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_FileOptions__fields[21] = {
    {1, UPB_SIZE(32, 32), 11, 0, 9, 1},
    {8, UPB_SIZE(40, 48), 12, 0, 9, 1},
    {9, UPB_SIZE(8, 8), 1, 0, 14, 1},
    {10, UPB_SIZE(16, 16), 2, 0, 8, 1},
    {11, UPB_SIZE(48, 64), 13, 0, 9, 1},
    {16, UPB_SIZE(17, 17), 3, 0, 8, 1},
    {17, UPB_SIZE(18, 18), 4, 0, 8, 1},
    {18, UPB_SIZE(19, 19), 5, 0, 8, 1},
    {20, UPB_SIZE(20, 20), 6, 0, 8, 1},
    {23, UPB_SIZE(21, 21), 7, 0, 8, 1},
    {27, UPB_SIZE(22, 22), 8, 0, 8, 1},
    {31, UPB_SIZE(23, 23), 9, 0, 8, 1},
    {36, UPB_SIZE(56, 80), 14, 0, 9, 1},
    {37, UPB_SIZE(64, 96), 15, 0, 9, 1},
    {39, UPB_SIZE(72, 112), 16, 0, 9, 1},
    {40, UPB_SIZE(80, 128), 17, 0, 9, 1},
    {41, UPB_SIZE(88, 144), 18, 0, 9, 1},
    {42, UPB_SIZE(24, 24), 10, 0, 8, 1},
    {44, UPB_SIZE(96, 160), 19, 0, 9, 1},
    {45, UPB_SIZE(104, 176), 20, 0, 9, 1},
    {999, UPB_SIZE(112, 192), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_FileOptions_msginit = {
    &google_protobuf_FileOptions_submsgs[0],
    &google_protobuf_FileOptions__fields[0],
    UPB_SIZE(120, 208),
    21,
    false,
};

static const upb_msglayout* const google_protobuf_MessageOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_MessageOptions__fields[5] = {
    {1, UPB_SIZE(1, 1), 1, 0, 8, 1},    {2, UPB_SIZE(2, 2), 2, 0, 8, 1},
    {3, UPB_SIZE(3, 3), 3, 0, 8, 1},    {7, UPB_SIZE(4, 4), 4, 0, 8, 1},
    {999, UPB_SIZE(8, 8), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_MessageOptions_msginit = {
    &google_protobuf_MessageOptions_submsgs[0],
    &google_protobuf_MessageOptions__fields[0],
    UPB_SIZE(12, 16),
    5,
    false,
};

static const upb_msglayout* const google_protobuf_FieldOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_FieldOptions__fields[7] = {
    {1, UPB_SIZE(8, 8), 1, 0, 14, 1},     {2, UPB_SIZE(24, 24), 3, 0, 8, 1},
    {3, UPB_SIZE(25, 25), 4, 0, 8, 1},    {5, UPB_SIZE(26, 26), 5, 0, 8, 1},
    {6, UPB_SIZE(16, 16), 2, 0, 14, 1},   {10, UPB_SIZE(27, 27), 6, 0, 8, 1},
    {999, UPB_SIZE(28, 32), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_FieldOptions_msginit = {
    &google_protobuf_FieldOptions_submsgs[0],
    &google_protobuf_FieldOptions__fields[0],
    UPB_SIZE(32, 40),
    7,
    false,
};

static const upb_msglayout* const google_protobuf_OneofOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_OneofOptions__fields[1] = {
    {999, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_OneofOptions_msginit = {
    &google_protobuf_OneofOptions_submsgs[0],
    &google_protobuf_OneofOptions__fields[0],
    UPB_SIZE(4, 8),
    1,
    false,
};

static const upb_msglayout* const google_protobuf_EnumOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_EnumOptions__fields[3] = {
    {2, UPB_SIZE(1, 1), 1, 0, 8, 1},
    {3, UPB_SIZE(2, 2), 2, 0, 8, 1},
    {999, UPB_SIZE(4, 8), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_EnumOptions_msginit = {
    &google_protobuf_EnumOptions_submsgs[0],
    &google_protobuf_EnumOptions__fields[0],
    UPB_SIZE(8, 16),
    3,
    false,
};

static const upb_msglayout* const google_protobuf_EnumValueOptions_submsgs[1] =
    {
        &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_EnumValueOptions__fields[2] = {
    {1, UPB_SIZE(1, 1), 1, 0, 8, 1},
    {999, UPB_SIZE(4, 8), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_EnumValueOptions_msginit = {
    &google_protobuf_EnumValueOptions_submsgs[0],
    &google_protobuf_EnumValueOptions__fields[0],
    UPB_SIZE(8, 16),
    2,
    false,
};

static const upb_msglayout* const google_protobuf_ServiceOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_ServiceOptions__fields[2] = {
    {33, UPB_SIZE(1, 1), 1, 0, 8, 1},
    {999, UPB_SIZE(4, 8), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_ServiceOptions_msginit = {
    &google_protobuf_ServiceOptions_submsgs[0],
    &google_protobuf_ServiceOptions__fields[0],
    UPB_SIZE(8, 16),
    2,
    false,
};

static const upb_msglayout* const google_protobuf_MethodOptions_submsgs[1] = {
    &google_protobuf_UninterpretedOption_msginit,
};

static const upb_msglayout_field google_protobuf_MethodOptions__fields[3] = {
    {33, UPB_SIZE(16, 16), 2, 0, 8, 1},
    {34, UPB_SIZE(8, 8), 1, 0, 14, 1},
    {999, UPB_SIZE(20, 24), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_MethodOptions_msginit = {
    &google_protobuf_MethodOptions_submsgs[0],
    &google_protobuf_MethodOptions__fields[0],
    UPB_SIZE(24, 32),
    3,
    false,
};

static const upb_msglayout* const
    google_protobuf_UninterpretedOption_submsgs[1] = {
        &google_protobuf_UninterpretedOption_NamePart_msginit,
};

static const upb_msglayout_field
    google_protobuf_UninterpretedOption__fields[7] = {
        {2, UPB_SIZE(56, 80), 0, 0, 11, 3}, {3, UPB_SIZE(32, 32), 4, 0, 9, 1},
        {4, UPB_SIZE(8, 8), 1, 0, 4, 1},    {5, UPB_SIZE(16, 16), 2, 0, 3, 1},
        {6, UPB_SIZE(24, 24), 3, 0, 1, 1},  {7, UPB_SIZE(40, 48), 5, 0, 12, 1},
        {8, UPB_SIZE(48, 64), 6, 0, 9, 1},
};

const upb_msglayout google_protobuf_UninterpretedOption_msginit = {
    &google_protobuf_UninterpretedOption_submsgs[0],
    &google_protobuf_UninterpretedOption__fields[0],
    UPB_SIZE(64, 96),
    7,
    false,
};

static const upb_msglayout_field
    google_protobuf_UninterpretedOption_NamePart__fields[2] = {
        {1, UPB_SIZE(8, 16), 2, 0, 9, 2},
        {2, UPB_SIZE(1, 1), 1, 0, 8, 2},
};

const upb_msglayout google_protobuf_UninterpretedOption_NamePart_msginit = {
    NULL,
    &google_protobuf_UninterpretedOption_NamePart__fields[0],
    UPB_SIZE(16, 32),
    2,
    false,
};

static const upb_msglayout* const google_protobuf_SourceCodeInfo_submsgs[1] = {
    &google_protobuf_SourceCodeInfo_Location_msginit,
};

static const upb_msglayout_field google_protobuf_SourceCodeInfo__fields[1] = {
    {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_SourceCodeInfo_msginit = {
    &google_protobuf_SourceCodeInfo_submsgs[0],
    &google_protobuf_SourceCodeInfo__fields[0],
    UPB_SIZE(4, 8),
    1,
    false,
};

static const upb_msglayout_field
    google_protobuf_SourceCodeInfo_Location__fields[5] = {
        {1, UPB_SIZE(24, 48), 0, 0, 5, 3}, {2, UPB_SIZE(28, 56), 0, 0, 5, 3},
        {3, UPB_SIZE(8, 16), 1, 0, 9, 1},  {4, UPB_SIZE(16, 32), 2, 0, 9, 1},
        {6, UPB_SIZE(32, 64), 0, 0, 9, 3},
};

const upb_msglayout google_protobuf_SourceCodeInfo_Location_msginit = {
    NULL,
    &google_protobuf_SourceCodeInfo_Location__fields[0],
    UPB_SIZE(40, 80),
    5,
    false,
};

static const upb_msglayout* const google_protobuf_GeneratedCodeInfo_submsgs[1] =
    {
        &google_protobuf_GeneratedCodeInfo_Annotation_msginit,
};

static const upb_msglayout_field google_protobuf_GeneratedCodeInfo__fields[1] =
    {
        {1, UPB_SIZE(0, 0), 0, 0, 11, 3},
};

const upb_msglayout google_protobuf_GeneratedCodeInfo_msginit = {
    &google_protobuf_GeneratedCodeInfo_submsgs[0],
    &google_protobuf_GeneratedCodeInfo__fields[0],
    UPB_SIZE(4, 8),
    1,
    false,
};

static const upb_msglayout_field
    google_protobuf_GeneratedCodeInfo_Annotation__fields[4] = {
        {1, UPB_SIZE(24, 32), 0, 0, 5, 3},
        {2, UPB_SIZE(16, 16), 3, 0, 9, 1},
        {3, UPB_SIZE(4, 4), 1, 0, 5, 1},
        {4, UPB_SIZE(8, 8), 2, 0, 5, 1},
};

const upb_msglayout google_protobuf_GeneratedCodeInfo_Annotation_msginit = {
    NULL,
    &google_protobuf_GeneratedCodeInfo_Annotation__fields[0],
    UPB_SIZE(32, 48),
    4,
    false,
};

#include "upb/port_undef.inc"

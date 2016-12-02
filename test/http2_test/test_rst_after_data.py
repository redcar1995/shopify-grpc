import http2_base_server

class TestcaseRstStreamAfterData(object):
  """
    In response to an incoming request, this test sends headers, followed by
    data, followed by a reset stream frame. Client asserts that the RPC failed.
    Client needs to deliver the complete message to the application layer.
  """
  def __init__(self):
    self._base_server = http2_base_server.H2ProtocolBaseServer()
    self._base_server._handlers['DataReceived'] = self.on_data_received
    self._base_server._handlers['SendDone'] = self.on_send_done

  def get_base_server(self):
    return self._base_server

  def on_data_received(self, event):
    self._base_server.on_data_received_default(event)
    sr = self._base_server.parse_received_data(event.stream_id)
    if sr:
      response_data = self._base_server.default_response_data(sr.response_size)
      self._ready_to_send = True
      self._base_server.setup_send(response_data, event.stream_id)
      # send reset stream

  def on_send_done(self, stream_id):
    self._base_server.send_reset_stream()
    self._base_server._stream_status[stream_id] = False

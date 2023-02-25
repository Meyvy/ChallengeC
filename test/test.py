import socket

# our defualt values
HOST = "127.0.0.1"
PORT = 8080
INT_PREFIX = "p1"
STRING_PREFIX = "p2"
RESPONSE_SIZE = 128


## a simple test function to test insert and connection
class Test:
    def __init__(self, host, port) -> None:
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)

    def check_connect(self):
        try:
            self.sock.connect((self.host, self.port))
        except:
            return False
        return True

    def insert(self, prefix, table, value):
        data = "{prefix}#{table}#{value}".format(
            prefix=prefix, table=table, value=value
        )
        self.sock.send(data.encode())
        data = self.sock.recv(RESPONSE_SIZE)
        if data[:2] == b"ok":
            return True
        return False


test = Test(HOST, PORT)

if not test.check_connect():
    print("test failed :no connection")
if not test.insert(STRING_PREFIX, "string_table", "are"):
    print("test failed: no insertion")

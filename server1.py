from socket import *
serverName="10.124.7.80"
serverPort=9999
serverSocket=socket(AF_INET,SOCK_STREAM)
serverSocket.bind((serverName,serverPort))
serverSocket.listen(1)
print("The server is ready")
while 1:
    connectionSocket,addr=serverSocket.accept()
    sentence=connectionSocket.recv(1024).decode
    file=open(sentence,"r")
    l=file.read(1024)
    connectionSocket.send(l.encode())
    file.close()
    connectionSocket.close()

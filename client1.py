from socket import *
serverName="10.124.7.80"
serverPort=9999
clientSocket=socket(AF_INET,SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence=input("Enter file name")
clientSocket.send(sentence.encode())
filecontents=clientSocket.recv(1024).decode
print("Front server:",filecontents)
clientSocket.close()

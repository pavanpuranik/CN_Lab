from socket import*
serverPort=12000
serverSocket=socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(("10.124.7.80",serverPort))
print("server is ready")
while 1:
    sentence,clientADDRESS=serverSocket.recvfrom(2048)
    file=open(sentence,"r")
    i=file.read(2048)
    
    file.close()
    connectionSocket.close()

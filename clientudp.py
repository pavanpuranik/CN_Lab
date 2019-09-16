from socket import*
serverName="10.124.7.80"
serverPort=12000
clientSocket=socket(AF_INET,SOCK_STREAM)
sentence=input("enter name")
clientSocket.sendto(bytes(sentence,"utf-8"),(serverName,serverPort))
filecontents,serverAddress=clientSocket.recvfrom(2048)
print('from server',filecontents)
clientSocket.close()
 

import socket

import menu


def start_connection(HOST,PORT):
    data=""
    try:
        s= socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        try:
            s.connect((HOST,PORT))
            print("connection to server established")
        except socket.error as e:
            print(str(e))
            return
    except ConnectionRefusedError:
        print("server might be down :(")
    #try:
      #  response=s.recv(1024)
   # except OSError:
       # print("theres a problem with the connection shuting down...")
    choice=""
    while True:
        operations=[]
        operation=menu.transaction_menu()
        operations.append(operation)
        print("these are your operations so far: ")
        #print(operations)
        for operation in operations:
            print(f"you are gonna {operation[0]} {operation[2]} from  the account of reference {operation[1]}")
        requests=[":".join(operation) for operation in operations]
        for request in requests:
            s.sendall(request.encode())
            response=s.recv(1024)
            print(response.decode('utf-8'))
        while choice!="y" and choice!="q":
            print("do you wish to make another transaction? press y to conitnue or q to quit")
            choice=input()
        if(choice=="y"):
            choice=""
            continue    
        elif choice=="q":
            break
    

def main():
    print("Welcome User!")
    start_connection("",9800)
    print("Thank you for using our service hope to see you soon Bye :) ")
    

main()

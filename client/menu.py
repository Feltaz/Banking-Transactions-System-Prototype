def transaction_menu():
    try:
        account={}
        account["operation"]=0
        account["montant"]=-1
        try:
            account["ref"]=int(input("please enter your account reference\n"))
        except KeyboardInterrupt:
            print("Interrupt from user revieve shuting down gracefully")
        while account["operation"]==0 and  account["operation"]!=1 and account["operation"]!=2:
            print("1:debit")
            print("2:credit")
            account["operation"]=int(input())
        if account["operation"]==1:
            account["operation"]="debit"
        elif account["operation"]==2:
            account["operation"]="credit"
        while account["montant"]<0:
            try:
                account["montant"]=int(input("write the amount for the operation\n"))
            except ValueError:
                print("please provide an integer thank you ")
        return account["operation"],str(account["ref"]),str(account["montant"])
    except KeyboardInterrupt:
        print("Interrupt recieved shuting down safely, GoodBye..\n")
        return -1 ##if function returned -1 means keyboard interrup happened 
    
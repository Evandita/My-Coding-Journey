def findAccount(lines, username, password, mode):
    for i in range(0, len(lines), 2):
        username2 = lines[i].strip()
        password2 = lines[i + 1].strip()
        if mode == 1 and username == username2 and password == password2:
            return True
        if mode == 2 and username == username2:
            return True
    return False

def login(username, password):
    with open("account_db.txt", 'r') as file:
        lines = file.readlines()
        if findAccount(lines, username, password, 1):
            print("Login successful")
        elif findAccount(lines, username, password, 2):
            print("Wrong password")
        else:
            print("Account not found")

def signup(username, password):
    with open("account_db.txt", 'a+') as file:
        file.seek(0)
        lines = file.readlines()
        if findAccount(lines, username, password, 2):
            print("Account already exists")
        else:
            file.write(username + '\n')
            file.write(password + '\n')
            print("Account successfully created")

choice = 0
while choice < 1 or choice > 2:
    print("1. Login Account")
    print("2. Signup Account")
    choice = int(input())

username = input("Username: ")
password = input("Password: ")

if choice == 1:
    login(username, password)
else:
    signup(username, password)

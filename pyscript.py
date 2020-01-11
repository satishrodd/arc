import subprocess
from fcntl import fcntl, F_GETFL, F_SETFL
from os import O_NONBLOCK, read
from time import sleep

def input_str(a,b):
    a += b
    a +="\n"
    return(a)

def insert_node(p, m):
    a = ""
    a = input_str(a, p)
    a = input_str(a, m)
    return(a)


def insert_nodes():
    a = ""
    #choose the option
    a = input_str(a, "1")
    #enter the number of nodes
    a = input_str(a, "11")
    #enter ip and prefix len
    a += insert_node("127.0.0.1", "24")
    a += insert_node("127.0.0.1", "32")
    a += insert_node("127.0.0.2", "32")
    a += insert_node("127.0.1.1", "24")
    a += insert_node("127.1.0.1", "16")
    a += insert_node("127.0.0.0", "8")
    a += insert_node("64.0.0.1", "24")
    a += insert_node("64.0.0.1", "32")
    a += insert_node("192.168.0.1", "24")
    a += insert_node("192.168.1.1", "24")
    a += insert_node("224.0.0.1", "8")
    return(a)

def subwalk(p, m):
    a = ""
    a = input_str(a, "4")
    a = input_str(a, p)
    a = input_str(a, m)
    return(a)

def delete_node(p, m):
    a = ""
    a = input_str(a, "2")
    a = input_str(a, p)
    a = input_str(a, m)
    return(a)

def long_match(p):
    a = ""
    a = input_str(a, "3")
    a = input_str(a, p)
    return(a)

    

def send_to_process(cmd):
    popen.stdin.write(cmd)
    #print popen.stdout.readline()
    return

# Test cases starts from here
#TC1: Insert 10 nodes of different prefix and prefix length.
def tc1():
    print "TC1: Insert prefixes"
    user_input = ""
    user_input += insert_nodes()
    user_input += subwalk("0.0.0.0", "0")
    send_to_process(user_input)
    return

#TC2: Delete a node
def tc2():
    print "TC2: Delete node"
    user_input =""
    user_input += delete_node("127.0.0.1", "24")
    user_input += subwalk("0.0.0.0", "0")
    send_to_process(user_input)
    return

#TC3: test the longest match
def tc3():
    print "TC3: Longest match"
    user_input =""
    user_input += long_match("64.0.0.0")
    send_to_process(user_input)
    return

def tc4():
    print "TC4: Subtree Walk"
    user_input =""
    user_input += subwalk("127.0.0.1", "8")
    send_to_process(user_input)
    return

def tc5():
    print "TC5: Subtree Walk"
    user_input =""
    user_input += subwalk("64.0.0.1", "8")
    send_to_process(user_input)
    return

def tc6():
    print "TC6: Subtree Walk"
    user_input =""
    user_input += subwalk("192.168.0.1", "24")
    send_to_process(user_input)
    return

def tc7():
    print "TC7: Insert node"
    user_input =""
    user_input = input_str(user_input, "1")
    user_input = input_str(user_input, "1")
    user_input += insert_node("127.0.0.1", "24")
    user_input += subwalk("0.0.0.0", "0")
    send_to_process(user_input)
    return

def tc8():
    print "TC8: Longest match"
    user_input =""
    user_input += long_match("192.168.2.1")
    send_to_process(user_input)
    return

def tc9():
    print "TC9: Delete non existent node"
    user_input =""
    user_input += delete_node("225.0.0.1", "8")
    send_to_process(user_input)
    return

def tc10():
    print "TC10: Insert node invalid prefix"
    user_input =""
    user_input = input_str(user_input, "1")
    user_input = input_str(user_input, "1")
    user_input += insert_node("300.300.300.300", "24")
    send_to_process(user_input)
    return

def tc11():
    print "TC11: Insert node invalid prefix len"
    user_input =""
    user_input = input_str(user_input, "1")
    user_input = input_str(user_input, "1")
    user_input += insert_node("127.0.0.1", "100")
    send_to_process(user_input)
    return


def run_tests():
    tc1()
    tc2()
    tc3()
    tc4()
    tc5()
    tc6()
    tc7()
    tc8()
    tc9()
    tc10()
    tc11()
    return

popen = subprocess.Popen("./bin",shell=False,bufsize=1024,
                         universal_newlines=True,
                         stdin=subprocess.PIPE)#, 
                         #stdout=subprocess.PIPE)
#flags = fcntl(popen.stdout, F_GETFL)
#fcntl(popen.stdout, F_SETFL, flags | O_NONBLOCK)

run_tests()
#print popen.stdout.read()
#close the process.
a = ""
a = input_str(a, "0")
send_to_process(a)

from matplotlib import pyplot as plt

x=["FIFO","RR","OTHER"]
f=open("output.txt","r")
data=f.readlines()
data=list(map(float,data))
f.close()
plt.bar(x,data,color="blue")
plt.yscale("log")
plt.show()
from tkinter import *

global shichangdaima,heyuedaima,zuixinjia,hqlist,symbol
shichangdaima=['0','0','0','0','0','0']
class GUI(object):
    def gui(self):  #主控件
        self.root=Tk()
        self.root.title('HqGUI')
        self.root.geometry('500x200')
        self.root.resizable(width=True,height=True) #控件大小可变动性

    def lable(self):    #显示合约主题的标签
        self.lable00=Label(self.root, text='代码    最新   成交量   涨跌   昨结   时间',
              fg='green').grid(row=0, sticky=W)
        self.lable01=Label(self.root)
        self.lable01.grid(row=1,sticky=W)
        self.lable02=Label(self.root,text='合约代码')
        self.lable02.grid(row=2,sticky=W)

        while True: #更新lable中的数据
                self.lable01['text'] = (shichangdaima[0], shichangdaima[1],
                                          shichangdaima[2], shichangdaima[3],
                                          shichangdaima[4], shichangdaima[5])
                self.root.update()

    def entry(self):    #输入框
        # var = StringVar
        # self.entry01=Entry(self.root,textvariable=var)
        self.entry01 = Entry(self.root) #获取行情代码，格式为XCZE,SR801
        self.entry01.grid(row=4,column=0,sticky=W)
        self.entry01.get()  # 获取输入的值

        self.entry02=Entry(self.root)   #获取行情IP地址格式为：127.0.0.1:11200
        self.entry02.grid(row=3, column=0, sticky=W)
        self.entry02.get()  # 获取输入的值

    def button(self):   #按钮
        self.button01=Button(self.root,text='订阅',anchor = 's',bg='green',command=CMDhqGUI.download)    #订阅按钮
        self.button01.grid(row=4,column=1)
        self.button02 = Button(self.root, text='连接', anchor='s',bg='green', command=CMDhqGUI.loginCMDS)  # 登录行情服务器
        self.button02.grid(row=3, column=1)

        self.quit=Button(self.root,text='退出',bg='red',command=self.root.destroy)
        self.quit.grid(row=5)

mygui = GUI()

def getstkcode():
    return mygui.entry01.get()
def getIP():
    return mygui.entry02.get()

def main():
        # t1 = threading.Thread(CMDhqGUI.main())
        # t1.start()
        # time.sleep(3)
        mygui.gui()

        mygui.entry()
        getstkcode()
        getIP()
        mygui.button()
        mygui.lable()
        #CMDhqGUI.Hqshow()
        mygui.root.mainloop()


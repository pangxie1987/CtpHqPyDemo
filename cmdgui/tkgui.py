#encoding=utf-8
import tkinter as tk
import tkinter.ttk
from configparser import ConfigParser
import data_type
import CMDhqGUI
from CMDhqGUI import hqdata
import logger
#from CMDhqGUI import stkcodelist
import threading
import time
import logging

path='../config.ini'


class CmdGui(object):
    def __init__(self):
        self.root=tk.Tk()
        self.root.title('CMD_GUI')
        self.root.resizable(width=True,height=True)
        self.root.geometry('1300x500')


        self.hqlistframe=tk.Frame( width=1300, height=400)
        self.hqlistframe.grid(row=0,column=0)

        #创建行情列表的Treeview
        self.hqtree=tk.ttk.Treeview(self.hqlistframe,show='headings',height=15,
                                    columns=('a','b','d','e','f','g','h','i','j','k','l',
                                             'm','n','o','p','q','r','s','t'))

        self.hqtree.column('a', width=60,anchor='w',)
        self.hqtree.column('b', width=150,anchor='w',)
        self.hqtree.column('d', width=60,anchor='w',)
        self.hqtree.column('e', width=60,anchor='w',)
        self.hqtree.column('f', width=60,anchor='w')
        self.hqtree.column('g', width=60,anchor='w')
        self.hqtree.column('h', width=60,anchor='w')
        self.hqtree.column('i', width=60,anchor='w')
        self.hqtree.column('j', width=60,anchor='w')
        self.hqtree.column('k', width=60,anchor='w')
        self.hqtree.column('l', width=60,anchor='w')
        self.hqtree.column('m', width=60,anchor='w')
        self.hqtree.column('n', width=60,anchor='w')
        self.hqtree.column('o', width=60,anchor='w')
        self.hqtree.column('p', width=70,anchor='w')
        self.hqtree.column('q', width=60,anchor='w')
        self.hqtree.column('r', width=60,anchor='w')
        self.hqtree.column('s', width=60,anchor='w')
        self.hqtree.column('t', width=60, anchor='w')


        self.hqtree.heading('a',text='市场')
        self.hqtree.heading('b', text='合约代码')
        self.hqtree.heading('d', text='状态')
        self.hqtree.heading('e', text='开盘价')
        self.hqtree.heading('f', text='最新价')
        self.hqtree.heading('g', text='最高价')
        self.hqtree.heading('h', text='最低价')
        self.hqtree.heading('i', text='成交量')
        self.hqtree.heading('j', text='涨跌')
        self.hqtree.heading('k', text='涨停板')
        self.hqtree.heading('l', text='跌停板')
        self.hqtree.heading('m', text='净持仓')
        self.hqtree.heading('n', text='昨日结算')
        self.hqtree.heading('o', text='今日结算')
        self.hqtree.heading('p', text='昨日持仓量')
        self.hqtree.heading('q', text='今日收盘')
        self.hqtree.heading('r', text='买总量')
        self.hqtree.heading('s', text='卖总量')
        self.hqtree.heading('t', text='成交金额')

        self.hqtree.grid(row=0,column=0)

        self.hqtree.tag_configure('ttk',background='yellow')
        self.hqtree.tag_bind('ttk','<1>',self.itemClicked)

        #更改treevie的背景色
        self.hqtree.tag_configure('oddrow', background='orange')
        self.hqtree.tag_configure('evenrow', background='purple')


        #创建滚动条
        self.sollbar=tk.Scrollbar(self.hqlistframe,orient=tk.VERTICAL, command=self.hqtree.yview)
        self.hqtree.configure(yscrollcommand=self.sollbar.set)
        self.sollbar.grid(row=0,column=1,sticky=tk.NS)

        #创建新的frame.防止操作控件
        self.frame02=tk.Frame(width=1200,height=100)
        self.frame02.grid(row=1,column=0)

        self.lable01=tk.Label(self.frame02,text='行情地址')
        self.lable01.grid(row=0,column=0)
        self.lable02 = tk.Label(self.frame02, text='合约列表')
        self.lable02.grid(row=1, column=0)
        self.lable03 = tk.Label(self.frame02, text='连接状态',font=('Helvetica', '14', 'bold'))
        self.lable03.grid(row=0, column=3)
        self.lable04 = tk.Label(self.frame02,fg='green',font=('Helvetica', '14', 'bold'))
        self.lable04.grid(row=0, column=4)

        self.entry01=tk.Entry(self.frame02,width=50)
        self.entry01.grid(row=0, column=1)
        self.entry01.get()
        self.entry02 = tk.Entry(self.frame02,width=50)
        self.entry02.grid(row=1, column=1)
        self.entry02.get()

        self.button01=tk.Button(self.frame02,text='连接',width=10,command=self.loginfuc)
        self.button01.grid(row=0,column=2)
        self.button02 = tk.Button(self.frame02, text='订阅',width=10,command=self.bookdata)
        self.button02.grid(row=1, column=2)
        self.button03=tk.Button(self.frame02,text='保存参数',width=10,command=self.saveparam)
        self.button03.grid(row=3, column=1)
        self.button04 = tk.Button(self.frame02, text='退出',width=10, command=self.root.destroy)
        self.button04.grid(row=4, column=1)


    def itemClicked(self,*args):
        print("click", self,args)


    def run(self):
        self.root.mainloop()

    #保存参数到文件
    def saveparam(self):
        config=ConfigParser()
        logger.logging.info('open config.ini')
        config.read(path)
        config.set('CMD','ip1',self.entry01.get())
        config.set('MKCode', 'mk', self.entry02.get())
        with open(path,'w') as f:
            config.write(f)
        logger.logging.info('保存CMD/ip1,MKCode/mk参数成功')

    #从配置文件中读取参数信息
    def readparam(self):
        config = ConfigParser()
        config.read(path)
        print('test------',config.get('CMD','ip1'))
        self.entry01.insert(0,config.get('CMD','ip1'))
        self.entry02.insert(0, config.get('MKCode', 'mk'))
        logger.logging.info('从配置文件按读取参数成功')

    #多线程处理登录事件，防止阻塞ui线程
    def loginfuc(self):
        #登录前获取信息
        data_type.datatype.hqip = self.entry01.get()
        t1=threading.Thread(CMDhqGUI.loginCMDS())
        t1.setDaemon(True)
        t1.start()

    #订阅行情
    def bookdata(self):
        #订阅前获取行情列表
        logger.logging.info('开始订阅行情')
        data_type.datatype.stkcode_list = self.entry02.get()
        t2=threading.Thread(CMDhqGUI.download())
        t2.setDaemon(True)
        t2.start()

    #将推送的行情数据更新到treeview中
    global stkcodelist
    stkcodelist=[]
    global index
    index={'key':'value'}

    def inserttreeview(self):
        global item

        if hqdata[0]!=0:    #行情回报中合约代码不为0时，更新到界面的treeview
            print('index', index)

            if hqdata[1] in stkcodelist:    #如果合约已经存在treeview中，则更新
                print('item=====',item)
                # self.hqtree.delete(index[hqdata[1]])  #删除整行数据
                self.hqtree.set(item, 4, hqdata[4])   #更新最新价
                self.hqtree.set(item, 5, hqdata[5])
                self.hqtree.set(item, 6, hqdata[6])
                self.hqtree.set(item, 7, hqdata[7])
                self.hqtree.set(item, 8, hqdata[8])
                self.hqtree.set(item, 11, hqdata[11])
                self.hqtree.set(item, 16, hqdata[16])
                self.hqtree.set(item, 17, hqdata[17])
                self.hqtree.set(item, 18, hqdata[18])
                logger.logging.info('更新行情,合约%s'%hqdata[1])

            else:       #如果是新合约，则插入数据
                item = self.hqtree.insert('', 1, values=(hqdata),tags=( 'oddrow'))
                logger.logging.info('插入新行情,合约%s' % hqdata[1])

                #hqdata[1] not in stkcodelist:
                stkcodelist.append(hqdata[1])
                index[hqdata[1]] = item
                print('------a------',item)

    def lable04update(self):
            self.lable04['text']=data_type.datatype.status
            self.root.update()


def main():
    logger.logging.info('start main gui')
    CMDhqGUI.cmdgui=CmdGui()
    CMDhqGUI.cmdgui.readparam()
    #cmdgui.inserttreeview()

    CMDhqGUI.cmdgui.lable04update()
    CMDhqGUI.cmdgui.run()


if __name__=='__main__':
    main()
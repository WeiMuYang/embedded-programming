# MMJK程序说明    

## 1 前言   
该项目是某等效设备的模拟，主要通过USB-5830的IO模块去控制灯光的效果。这个只是一个设备测试的程序，通过数据输入控制数据输出的效果。     

## 2 运行平台   
- 操作系统：Win10，Ubuntu16.04      
- 开发环境：qtcreator    
- 开发库：Qt5，USB5830的函数库   
- 涉及技术点：多线程，IO控制，信号槽，界面设计等   

## 3 软件功能   
多线程控制输出端口的输出    

## 4 软件实现   
1. 包含三个类，作用如下：
	- IO的控制类`IOCtrl`：通过这个类来初始化设备，建立设备的IO对象，同时启动线程，销毁IO对象   
	- 数据输出端口线程类`DOThr`：对应一个数据输出的端口    
2. 本例子涉及到4个端口的测试      

## 5 遇到问题及思考    
1. 数据的输出可以通过多线程来控制，这比通过一个总的端口控制要方便很多   
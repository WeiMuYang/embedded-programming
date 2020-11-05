# Ubuntu18.04的安装及环境配置   

## 1 系统安装    
1. Ubuntu官网下载系统镜像   
2. 做系统盘      
3. 设置U盘启动    
4. 安装Ubuntu系统   


## 2 配置环境    
1. 更新系统   
```shell
sudo apt-get update
```

2. 安装qt5   
```shell
sudo apt-get install qt5-default qtcreator
```

3. 安装新立得  
```shell
apt-get install synaptic
```

4. 通过新立得安装qt4    

5. 安装Typroa   
```shell
wget -qO - https://typora.io/linux/public-key.asc | sudo apt-key add -

# add Typora's repository  
sudo add-apt-repository 'deb https://typora.io/linux ./'
sudo apt-get update

# install typora  
sudo apt-get install typora

# upgrade all packages include Typora
sudo apt-get upgrade
```

6. 安装git   
```shell
sudo apt install git
```

7. 安装ssh   
```shell 
sudo apt-get update  
sudo apt-get install openssh-client 
sudo apt-get install openssh-server 
sudo service ssh start  
```

8. 安装vim   

```shell
sudo apt install vim
```

9. 安装gcc，g++，gdb，make   
```shell
sudo apt update
sudo apt install build-essential
gcc --version
sudo apt-get install gbd 
```

10. 安装smb   

```shell
sudo apt-get install samba  
```


1 编译说明
    2.1 创建build编译文件夹
        mkdir build
        cd build
    
    2.2 制定编译平台
        x86：cmake ../ -DPF=x86
        mips: cmake ../
        
    2.3 编译
        make
    
    2.4 编译生成文件
        lib文件夹: 放置各模块include文件及库文件
        bin文件夹：放置各模块二进制文件

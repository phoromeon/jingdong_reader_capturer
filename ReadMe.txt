本程序开发平台：Microsoft Visual Studio

编写此程序的目的，也是因为京东阅读购买的书籍有次数以及终端数的限制，为避免系统重装或更换硬件以后需要续费才能继续阅读书籍的麻烦，编写这个程序方便将购买的书籍保存到本地硬盘。

运行此程序时，请确保已经运行京东阅读并且已经正在阅读一本书籍，即打开了阅读书籍的窗口。由于并非智能软件，只是傻瓜式地截图，即冗余的工作由机器完成，因此，需要先输入正在阅读的书籍的名字，对于这个程序来说，就相当于找到书籍对应窗口的句柄。最好将窗口最大化，因为本程序会在截图的时候将阅读窗口最大化，而京东阅读窗口最大化以及正常窗口所显示的页数是不同的，因为涉及到输入总页数的问题，如果打开的时候是正常窗口，即没有最大化，此时输入最大页数的话，运行截图功能会再次将窗口最大化，而最大化后总页数会减少，从而导致已经截完最后一页了，而程序仍然在继续截取最后一页，造成重复且冗余。

由于不够智能，截图保存下来均为图片文件，需要整合成pdf或modi书籍仍需借助其它工具来完成。

另外，编写此程序还需要借助OpenCV开发包，因为涉及到图片保存格式的问题，笔者没有找到其它开源而且好用的开发包，而且因为年代久远，已经没办法找到上古的GDI+，如果用MFC自带的保存位图功能，那保存下来的图片将会是原图，截取一本书下来保存的图片无论是数据量还是容量都会非常大，因此需要借助OpenCV来保存图片文件。

由于用的是VC++ 6.0 ，所以对应的OpenCV版本是1.0.0，可以在Sourceforge.net上找到源代码，通过CMake工具编译成1.0.0版本，如果嫌麻烦，也可以在网上找已编译好的版本。

本程序代码主要用于交流学习，生成的书籍仅供自己阅读，切忌做成书籍后用于商业用途。
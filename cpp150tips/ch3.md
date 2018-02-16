# 第三章 说一说“内存管理”的那些事儿

## Quiz 神奇的 **烫** **屯** 来自何方

在VC中，栈空间未初始化的字符默认是-52，补码是0xCC,两个0xCC，即0xCCCC在GBK编码中就是“烫”，堆空间为初始化的字符默认是-51，两个-51的补码在GBK编码中为“屯”，二者都是未初始化的内存。


## Tip27 区分内存分配的方式

程序加载到内存后，会形成一个运行区间，依次可分为

1. **代码区/Code Area** 存放程序的执行代码
2. **数据区/Data Area** 存放的是全局变量、常量、静态变量等
3. **堆区/Heap Area** 存放的是动态内存,由new分配的内存块，如果在程序中不使用delete释放**，会在程序结束后由系统自动回收；
4. **栈区/Stack Area** 存放的是程序用的局部变量,比如函数内的局部变量，它在函数结束后会被自动释放


在C++中，数据区可以被分为自由存储区、全局/静态存储区和常量存储区，具体如下：

1. **自由存储区** 由malloc等分配的内存块，和堆类似，但由free释放
2. **全局/静态存储区** 全局变量和静态变量被分配到统一内存块中，值得注意的是，在C中，全部变量又被分为初始化的和为初始化的
3. **常量存储区** 比较特殊，存的是常量，不允许修改

堆和栈的主要区别
1. **管理方式** 堆由程序猿控制，栈由编译器控制
2. **空间大小** 在32位系统下，堆大小可达 2^32B/4GB大小，而栈一般是有空间大小限制
3. **碎片问题** 在堆中，频繁的new/delete会造成内存空间的不连续
4. **生长方向** 对于堆，生长方向向上，即向着内存地址增加爱的方向增长，对于栈，生长方向向下，即向着内存地址减少的方向增长
5. **分配方式** 堆都是动态分配，栈是由编译器动态分配的
6. **分配效率** 



## Tip 28 new/delete与new[]/delete[]必须配对使用

new([])/delete([])的操作中，C++需要知道指针所指内存块的大小，否则无法正确的释放分配的内存空间。在使用new[]分配一个数组指针时，编译器在内存中保留了数组元素的个数信息

## Tip 29 区分new的三种形态

1. new operator 
2. operator new
3. placement new



## Tip 30 new内存分配失败后的正确处理

**AHA** 640KB ought to be enough for everyboday.    Bill Gates.

在C和C++早期，Test-for-NULL用来检测new分配内存是否成功，但新时期的的new在申请内存失败时，会抛出一个bad_alloc exception,这样在新的C++标准中，Test-for-NULL这种处理方式不再被推荐，同时为了兼容以前的Test-for-NULL代码，C++标委会提供了operator new的一种可选参数 nothrow。这样，如果 operator new的nothrow版本调用失败，operator new 返回null指针。


## Tip 31 了解new_handler的所作所为

operator new 申请内存失败后，编译器会调用一个错误处理函数/new-handler进行相应处理，好的new-handler函数需要遵循以下策略之一：
1. Make more memory available,operator new在进行多次的内存分配时，可能会尝试使其下一次分配的内存更大，然后在new-handler第一次被调用到时释放它供程序使用；
2. Install a different new-handler,加入当前new-handler不能获得更多的内存供operator new分配使用，而另一个new-handler可以时，当前new-handler会调用set_new_handler在它的位置上安装另一个new-handler，这样下次 operator new在调用new-handler时，会调用最新安装的那个；
3. Deinstall the new-handler,即将空指针传给set_new_handler,也就没有了相应的new-handler,当内存分配失败时，operator new会抛出一个异常
4. Throw an exception,跑出一个类型为bad_alloc或继承自bad_alloc的异常
5. Not return,直接调用abort或exit结束应用程序

这些各式各样的new-handler函数是通过调用标准库函数set_new_handler进行特殊定制的，该库函数位于 <new\>中。如果需要对 class-sepeicfic new-handler支持，需要为每一个class提供专属的set_new_handler和operator new版本，以下以class A为例：

    class A
    {
	public:
		static std::new_handler set_new_handler(std::new_handler p)throw()
		{
			std::new_handler oldHandler = m_currHandler;
			m_currHandler = p;
			return oldHandler;
		}

		static void* operator new(std::size_t size)throw(std::bad_alloc)
		{
			set_new_handler(MemoryErrorHanding);
			return ::operator new(size);
		}

		static void MemoryErrorHandling()
		{
			//error handling code
		}
	private:
		static std::new_handler m_currHandler;
    }


## Tip 32 借助工具检测内存泄漏问题

内存泄漏主要指的是堆内存泄露，检测内存泄露的关键是要能截获对分配内存和释放内存的函数的调用，用以跟踪每一块内存的生命周期。

1. 内嵌式
   MS C-Runtime Library：
2. 外挂式
   Windows平台：BoundsChecker,Insure++;
   Linux平台：Rational Purify,Valgrind


## Tip 33  小心翼翼的重载operator new/operator delete


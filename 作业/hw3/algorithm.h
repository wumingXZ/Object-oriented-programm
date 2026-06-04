#ifndef GUARD_median_h
#define GUARD_median_h

#include <vector>

//以下是自定义的模板函数
//均在自定义名字空间MySpace中。
//需要使用到这些函数时，
//可在文件开头使用"using MySpace::find;"等，
//或直接在函数名前加"MySpace::"。
namespace MySpace{

//自定义模板函数swap：交换两个变量的值 
template <class X>
void swap(X & x, X & y){
	X tmp=x;
	x=y;
	y=tmp;
}

//自定义模板函数sort：用冒泡法排序 
template<class Ran>
void sort(Ran begin, Ran end)
{
//	std::cout << "sort...\n";
	for(Ran i=begin;i!=end;i++)
		for(Ran j=begin;j+1+(i-begin)!=end;j++)
			if(*j>*(j+1))
				swap(*j,*(j+1));
}

//自定义模板函数median：求中值 
template<class T>
T median(std::vector<T> vec)
{
//	std::cout << "median...\n";
	typedef typename std::vector<T>::size_type vec_sz;
	vec_sz size = vec.size();
	if(size == 0)
		throw std::domain_error("median of an empty vector");
	MySpace::sort(vec.begin(),vec.end());
	vec_sz mid = size/2;
	return size % 2 == 0 ? (vec[mid]+vec[mid-1])/2 : vec[mid];
}

//自定义模板函数average：求均值
template<class T>
T average(std::vector<T> vec)
{
//	std::cout << "average...\n";
	//TODO
	auto size = vec.size();
	if(size == 0) {
		throw std::domain_error("average of an empty vector");
	}	
	T sum = 0;
	for(int i = 0; i < size; ++i) {
		sum += vec[i];	
	}
	
	return sum / size;
}

//自定义模板函数find：实现查找功能 
template <class In, class X> 
In find(In begin, In end, const X&x){
//	std::cout << "find...\n";
	//TODO
	for(; begin != end; begin++) {
		if (*begin == x) {
			return begin;
		}
	}
	
	return end;
}

//自定义模板函数remove_copy：
//将不等于x的元素从原处复制到以dest表示的目标处。 
template <class In, class Out, class X> 
Out remove_copy(In begin, In end, Out dest,const X&x){
//	std::cout << "remove_copy...\n";
	for(; begin != end; begin++) {
		if (*begin != x) {
			*dest = *begin;
			dest++;
		}
	}
	return dest;
}

//自定义模板函数transform：
//对以begin和end表示的序列中的元素调用函数f，
//并存储到以d表示的目标处。 
template <class In, class Out, class F> 
Out transform(In begin,In end, Out dest, F func){
//	std::cout << "transform...\n";
	//TODO
	for(; begin != end; begin++, dest++) {
		*dest = func(*begin);
	}

	return dest;
}

}
#endif

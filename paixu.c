#include <stdio.h>
//冒泡排序//
int main()
{
      int a[] = {12, 43, 1, 3, 6, 5, 4, 7, 9, 8}; //这里要注意的是数组的下标是从0开始的
      int n = sizeof(a) / sizeof(a[0]);           //存放数组a中元素的个数//
      int buf;
      for (int i = 0; i < n - 1; i++)
   
  {
            for (int j = 0; j < n - 1 - i; j++)
       
    {
                  if (a[j] < a[j + 1])
           
      {
                        buf = a[j];
                        a[j] = a[j + 1];
                        a[j + 1] = buf;
                   
      }
             
    }
       
  }
      for (int i = 0; i < n; i++)
   
  {
            printf("%d ", a[i]);
       
  }
      printf("\n");
      return 0;
}

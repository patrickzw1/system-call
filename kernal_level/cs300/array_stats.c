#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/limits.h>
#include <linux/uaccess.h>

asmlinkage long sys_array_stats(struct array_stats *stats, long data[], long size)
{
	//set up all the variable since it compiles by c90
	long input = 0;
	struct array_stats output;
	int i;
	output.min = LONG_MAX;
	output.max = LONG_MIN;
	output.sum = 0;

	//check the input are all valid
	if (size <= 0)
	{
		return -EINVAL;
	}
	if (stats == NULL)
	{
		return -EFAULT;
	}
	if (data == NULL)
	{
		return -EFAULT;
	}

	//in the loop, keep extract the number from data
	for (i = 0; i < size; ++i)
	{
		//use copy_from_user to avoid the bad input
		if (copy_from_user(&input, &data[i], sizeof(long)))
		{
			return -EFAULT;
		}
		else
		{
			//if input is valid, then add to sum and compare with max and min
			output.sum = output.sum + input;
			if (input > output.max)
			{
				output.max = input;
			}
			if (input < output.min)
			{
				output.min = input;
			}
		}
	}
	//outside the loop, means go through all the elements in data,
	//send result back to user using copy_to_user
	if (copy_to_user(stats, &output, sizeof(output)))
	{
		return -EFAULT;
	}
	return 0;
}

#include "process_ancestors.h"
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/limits.h>
#include <linux/uaccess.h>
#include <linux/cred.h>


asmlinkage long sys_process_ancestors(
                struct process_info info_array[],
                long size,
                long *num_filled)
{
  // using c90
  struct task_struct* cur_task = current;
  struct process_info pro_info;
  struct list_head* child_head = NULL;
  struct list_head* sibling_head = NULL;
  long num_children = 0;
  long num_sibling = 0;
  long i = 0;

  // check the input value is valid
  if (size <= 0 ) return -EINVAL;
  if (info_array == NULL || num_filled == NULL) return -EFAULT;

  // child_head = &(cur_task -> children);
  // sibling_head = &(cur_task -> sibling);

  // // initial the index 0 value into the array
  // pro_info.pid = cur_task -> pid;  //get the pid
  // pro_info.uid = (long)cur_task -> cred -> uid.val; // get the uid
  // strncpy(pro_info.name, cur_task -> comm, sizeof(pro_info.name)); //get the name of the process
  // pro_info.state = cur_task -> state; // get the current process state
  // pro_info.nvcsw = cur_task -> nvcsw; // get the # of  voluntary context switches
  // pro_info.nivcsw = cur_task -> nivcsw; // get the # of involuntary context switches

  // while(child_head -> next != &(cur_task -> children) ){
  //   num_children++;
  //   child_head = child_head -> next;
  // }
  // pro_info.num_children = num_children;

  // while(sibling_head -> next != &(cur_task -> sibling)){
  //   num_sibling++;
  //   sibling_head = sibling_head -> next;
  // }
  // pro_info.num_siblings = num_sibling;

  // cur_task = cur_task -> parent;

  // if(copy_to_user(&info_array[0], &pro_info, sizeof(pro_info))) return -EFAULT;
  // fill the process_info

  //set variable to the pro_info
    while (cur_task -> parent != cur_task){
      //check if out of the size, then break
      if (i > size) break;

      num_children = 0;
      num_sibling = 0;
      //update the pro_info
      pro_info.pid = cur_task -> pid;
      pro_info.uid =(long) cur_task ->  cred -> uid.val;
      //copy the string from src to dest of size of src
      strncpy(pro_info.name, cur_task -> comm, sizeof(pro_info.name));
      pro_info.state = cur_task -> state;
      pro_info.nvcsw = cur_task -> nvcsw;
      pro_info.nivcsw = cur_task -> nivcsw;

      //find the the list of children and sibling
      child_head = &(cur_task -> children);
      sibling_head = &(cur_task -> sibling);

      //count children and siblings when they are not equal to themselves
      while(child_head -> next != &(cur_task -> children) ){
        num_children++;
        child_head = child_head -> next;
      }
      pro_info.num_children = num_children;

      while(sibling_head -> next != &(cur_task -> sibling)){
        num_sibling++;
        sibling_head = sibling_head -> next;
      }
      pro_info.num_siblings = num_sibling;

      // printk("--------------------test in side the syscall --------------\n");
      // printk("pid 			=>  %ld\n", pro_info.pid);
      // printk("name 			=>  %s\n", pro_info.name);
      // printk("state			=>  %ld\n", pro_info.state);
      // printk("uid 			=>  %ld\n", pro_info.uid);
      // printk("nvcsw 		=>  %ld\n", pro_info.nvcsw);
      // printk("nivcsw 		=>  %ld\n", pro_info.nivcsw);
      // printk("num_children 	=>  %ld\n", pro_info.num_children);
      // printk("num_sibling 	=>  %ld\n", pro_info.num_siblings);
      // printk("--------------------end of the syscall test----------------\n");

      if(copy_to_user(&info_array[i], &pro_info, sizeof(pro_info))) return -EFAULT;

      cur_task = cur_task -> parent;
      i++;
    }
    if(copy_to_user(num_filled, &i, sizeof(i))) return -EFAULT;
    // printk("num_filled => %ld\n", i);
    // printk("--------------------END OF SYSTEMCALL------------------------\n");

    return 0;
}

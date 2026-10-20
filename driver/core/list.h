/**
 * @file    list.h
 * @brief   实现双向链表的功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年3月1日
 */

#ifndef __LIST_H_
#define __LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * 双向链表节点
 */
struct list_node{
    struct list_node *prev; /* 指向上一个节点 */
    struct list_node *next; /* 指向下一个节点 */
};
typedef struct list_node list_t;

/**
 * @function nane: list_init
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              node 链表节点地址
 * @output param:
 *              无
 * @description: 
 * 初始化链表节点
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline void list_init(list_t *node)
{
    node->next = node->prev = node;
}

/**
 * @function nane: list_insert_after
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              list 一个链表节点；一般是链表头，也可以是普通节点，如果为链表头则是在链表开头插入一个节点
 *              node 插入的新节点
 * @output param:
 *              无
 * @description: 
 * 在节点list节点的后面插入一个节点
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline void list_insert_after(list_t *list, list_t *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

/**
 * @function nane: list_insert_before
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              list 一个链表节点；一般是链表头，也可以是普通节点，如果为链表头则是在链表尾部插入一个节点
 *              node 插入的新节点
 * @output param:
 *              无
 * @description: 
 * 在节点list节点的前面插入一个节点
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline void list_insert_before(list_t *list, list_t *node)
{
    list->prev->next = node;
    node->prev = list->prev;

    list->prev = node;
    node->next = list;
}

/**
 * @function nane: list_remove
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              node 要删除的节点，不能是链表头，否则链表会失控
 * @output param:
 *              无
 * @description: 
 * 将节点从某个链表中删除
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline void list_remove(list_t *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    node->next = node->prev = node;
}

/**
 * @function nane: list_isempty
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              list 一个链表节点；一般是链表头，也可以是普通节点，如果是普通节点则是判断该节点是否孤立
 * @output param:
 *              无
 * @description: 
 * 判断链表是否为空
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline int32_t list_isempty(const list_t *list)
{
    return list->next == list ? 1 : 0;
}

/**
 * @function nane: list_length
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              list 一个链表节点，一般是链表头
 * @output param:
 *              return 返回链表中元素个数
 * @description: 
 * 获取链表中元素个数
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
static inline int32_t list_length(const list_t *list)
{
    int32_t length = 0;
    const list_t *tmp = list;

    while(tmp->next != list) {
        tmp = tmp->next;
        length++;
    }

    return length;
}

/**
 * @function nane: list_for_each
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              tmp 临时指针变量，用于循环遍历的中间变量，每次遍历到的节点都通过该指针获取
 *              list 链表头
 * @output param:
 *              无
 * @description: 
 * 实现链表的遍历，千万不能删除遍历到的节点
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
#define list_for_each(tmp, head) \
    for ((tmp) = (head)->next; (tmp) != (head); (tmp) = (tmp)->next)

/**
 * @function nane: list_for_each_safe
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              tmp 临时指针变量，用于循环遍历的中间变量，每次遍历到的节点都通过该指针获取
 *              n 临时指针变量，用于循环遍历的中间变量，内部使用
 *              head 链表头
 * @output param:
 *              无
 * @description: 
 * 实现链表的遍历，可以删除遍历到的节点
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
#define list_for_each_safe(tmp, n, head) \
    for ((tmp) = (head)->next, (n) = (tmp)->next; (tmp) != (head); (tmp) = (n), (n) = (tmp)->next)

/**
 * @function nane: list_for_each_c99
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              tmp 临时指针变量的名称，用于循环遍历的中间变量，每次遍历到的节点都通过该指针获取
 *              head 链表头
 * @output param:
 *              无
 * @description: 
 * 实现链表的遍历，可以删除遍历到的节点，此宏定义需要c99以上编译器支持
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
#define list_for_each_c99(tmp, head) \
    for (list_t *tmp = (head)->next, *n = tmp->next; tmp != (head); tmp = n, n = tmp->next)

/**
 * @function nane: container_of
 * @author: liaofei
 * @data: 2021年3月1日
 * @input param:
 *              ptr 成员变量地址
 *              type 结构体数据类型
 *              member 成员变量名称
 * @output param:
 *              return 成员所在结构体的地址
 * @description: 
 * 根据成员地址、结构体类型、成员名查找对应结构体地址
 * @modeify log: 
 * 2021年3月1日 liaofei 首次编写
 */
#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LIST_H_ */

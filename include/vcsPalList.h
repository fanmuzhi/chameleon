/*! @file vcsPalList.h
 *******************************************************************************
 **
 **                           NDA AND NEED-TO-KNOW REQUIRED
 **
 *****************************************************************************
 **
 **  Copyright (C) 2006-2016 Synaptics Incorporated. All rights reserved.
 **
 **
 ** This file contains information that is proprietary to Synaptics
 ** Incorporated ("Synaptics"). The holder of this file shall treat all
 ** information contained herein as confidential, shall use the
 ** information only for its intended purpose, and shall not duplicate,
 ** disclose, or disseminate any of this information in any manner unless
 ** Synaptics has otherwise provided express, written permission.
 ** Use of the materials may require a license of intellectual property
 ** from a third party or from Synaptics. Receipt or possession of this
 ** file conveys no express or implied licenses to any intellectual
 ** property rights belonging to Synaptics.
 **
 **
 ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
 ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
 ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
 ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
 ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
 ** OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY TO ANY PARTY
 ** SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
*/


/*!
*******************************************************************************
**  USDK PAL List Subsystem Definitions
**
**  This file contains the USDK PAL List Subsystem definitions.
**
** 
*/

#ifndef __vcsPalList_h__
#define __vcsPalList_h__

/* -------------------------------------------------------------------------------------
** List routines
*/

/* Private -----------------------------------
*/
#define _LIST_POISON1  ((palListEntry_t *) 0x00100100)
#define _LIST_POISON2  ((palListEntry_t *) 0x00200200)

typedef struct palListEntry_s {
    struct palListEntry_s * next;
    struct palListEntry_s * prev;
} palListEntry_t;

/* constant list*/
typedef struct palListEntryConst_s {
    const struct palListEntryConst_s * next;
    const struct palListEntryConst_s * prev;
} palListEntryConst_t;

typedef struct palListIterator_s {
    palListEntry_t *        pItNext;
    palListEntry_t *        pEntry;
} palListIterator_t;

#define PAL_ITERATOR_INIT   {0}

/* iterator for const lists */
typedef struct palListIteratorConst_s {
    const palListEntryConst_t *        pItNext;
    const palListEntryConst_t *        pEntry;
} palListIteratorConst_t;

#define _listAdd(_prev_, _next_, _new_) \
{ \
    palListEntry_t *_p = _prev_; \
    palListEntry_t *_n = _next_; \
    _n->prev = (_new_); \
    (_new_)->next = _n; \
    (_new_)->prev = _p; \
    _p->next = (_new_); \
}

/* for constant lists */
#define _listAddConst(_prev_, _next_, _new_) \
{ \
    palListEntryConst_t *_p = _prev_; \
    palListEntryConst_t *_n = _next_; \
    _n->prev = (_new_); \
    (_new_)->next = _n; \
    (_new_)->prev = _p; \
    _p->next = (_new_); \
}

#define _listRemove(_prev_, _next_) \
{ \
    palListEntry_t *_p = _prev_; \
    palListEntry_t *_n = _next_; \
    _n->prev = _p; \
    _p->next = _n; \
}

/* for const lists */
#define _listRemoveConst(_prev_, _next_) \
{ \
    palListEntryConst_t *_p = _prev_; \
    palListEntryConst_t *_n = _next_; \
    _n->prev = _p; \
    _p->next = _n; \
}

#define _listRemove2(list, entry) \
    (((entry) == (list)) ? VCS_NULL : (entry)); \
    _listRemove((entry)->prev, (entry)->next)

/* for const lists */
#define _listRemove2Const(list, entry) \
    (((entry) == (list)) ? VCS_NULL : (entry)); \
    _listRemoveConst((entry)->prev, (entry)->next)

/* Public  -----------------------------------
*/

/* palListInitialize
**
** A list starts out as a single palListEntry_t. Initialize that palListEntry_t
** such that it points back on itself and is ready for list-elements to be linked
** in. Each list-element to be linked in must contain a palListEntry_t structure
** within it.
**
** eg:
**
**    {
**        palListEntry_t pending;
**        palListEntry_t busy;
**
**        palListInitialize(&pending);
**        palListInitialize(&busy);
**    }
*/
#define palListInitialize(list) \
    (list)->next = (list)->prev = list

/* palListAddHead
**
** Add a list-element to the head/beginning of the specified list.
**
** eg: See palListAddTail
*/
#define palListAddHead(list, element) \
    _listAdd(list, (list)->next, element)

/* for const lists */
#define palListAddHeadConst(list, element) \
    _listAddConst(list, (list)->next, element)

/* palListAddTail
**
** Add a list-element to the tail/end of the specified list.
**
** eg:
**
**    typedef struct {
**        listEntry_t   list;
**        vcsUint32_t   msgId;
**    } msg_t;
**
**    {
**        msg_t          msg0;
**        msg_t          msg1;
**        palListEntry_t pending;
**        palListEntry_t busy;
**
**        palListInitialize(&pending);
**        palListInitialize(&busy);
**
**        msg.msgId = 0;
**        palListAddHead(&pending, &(msg0.list));
**
**        msg.msgId = 1;
**        palListAddTail(&pending, &(msg1.list));
**    }
*/
#define palListAddTail(list, element) \
    _listAdd((list)->prev, list, element)

/* for const lists */
#define palListAddTailConst(list, element) \
    _listAddConst((list)->prev, list, element)

/* palListRemoveHead
**
** eg: See palListRemoveTail
*/
#define palListRemoveHead(list) \
    _listRemove2(list, (list)->next)

/* for const lists */
#define palListRemoveHeadConst(list) \
    _listRemove2Const(list, (list)->next)

/* palListRemoveTail
**
** eg:
**
**    typedef struct {
**        listEntry_t   list;
**        vcsUint32_t   msgId;
**    } msg_t;
**
**    {
**        msg_t            msg0;
**        msg_t            msg1;
**        palListEntry_t   pending;
**        palListEntry_t   busy;
**        palListEntry_t * pEntry;
**
**        palListInitialize(&pending);
**        palListInitialize(&busy);
**
**        msg.msgId = 0;
**        palListAddHead(&pending, &(msg0.list));
**
**        msg.msgId = 1;
**        palListAddTail(&pending, &(msg1.list));
**
**        pEntry = palListRemoveHead(&pending);
**        pEntry = palListRemoveTail(&pending);
**
**    }
*/
#define palListRemoveTail(list) \
    _listRemove2(list, (list)->prev)

/* for const lists */
#define palListRemoveTailConst(list) \
    _listRemove2Const(list, (list)->prev)

/* palListIsEmpty
**
** Are there any entries in the list?
*/
#define palListIsEmpty(list) \
    ((!(palListEntryIsListItem(list))) || ((list)->next == (list)))

/* palListContainerOf
*/
#define palListContainerOf(ptr, type, member) \
    ((ptr) ? ((type *) ((char *) (ptr) - (VCS_OFFSETOF(type, member)))) : VCS_NULL)

/* palListEntry
**
*/
#define palListEntry(ptr, type, member) \
    palListContainerOf(ptr, type, member)

/* palListEntryIsListItem
**
** Checks the entry is owned to some list.
*/

#define palListEntryIsListItem(list) \
    ((list)->next != VCS_NULL && (list)->prev != VCS_NULL)

/* palListRemoveEntry
**
** Remove a single list-element from the list that it is linked into.
*/
#define palListRemoveEntry(entry) \
    _listRemove((entry)->prev, (entry)->next); \
    { (entry)->next = _LIST_POISON1; (entry)->prev = _LIST_POISON2; }

/* for const lists */
#define palListRemoveEntryConst(entry) \
    _listRemoveConst((entry)->prev, (entry)->next); \
    { (entry)->next = _LIST_POISON1; (entry)->prev = _LIST_POISON2; }

/* palListForEach
**
** Iterate through all of the entries in the given list.
*/
#define palListForEach(it, list)                                           \
    for ( (it).pEntry = (list)->next, (it).pItNext = ((it).pEntry)->next;   \
         (it).pEntry != (list);                                            \
         (it).pEntry = (it.pItNext), (it).pItNext = ((it).pEntry)->next)

#define palListGetHead(list)                                                \
        (list)->next

#define palListIteratorEntry(_it_, _type_, _member_) \
            palListEntry((_it_).pEntry, _type_, _member_)

#endif /* __vcsPalList_h__ */


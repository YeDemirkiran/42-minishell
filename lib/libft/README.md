# libft (a.k.a. Lib Forty-Two)
My own custom implementation of the libft library - by [yademirk][yademirk-url].

This version of libft is NOT the one I submitted the libft project on Ecole 42. Instead, it's a heavily modified version that suits to my needs on each 42 project. It gets updated and new functions are added with each project I start. Therefore, it doesn't follow the usual libft structure and may contain additional functions (ft_printf, get_next_line, etc). Due to this, it's NOT suitable for passing the first libft project.

However, you can use it for learning purposes.

# Submodules
The library consists of 8 submodules.

## 1 - Char
Consists of char modification functions.

- [ft_tolower()](./src/char/ft_tolower.c): Converts an uppercase character to lowercase and returns the converted char.
- [ft_tolower()](./src/char/ft_toupper.c): Converts a lowercase character to uppercase and returns the converted char.

## 2 - Converter
Consists of conversion functions that converts one data type to another.

- [ft_atoi()](./src/converter/ft_atoi.c): Converts an ASCII array to integer.
- [ft_itoa()](./src/converter/ft_itoa.c): Converts an integer to an ASCII string.

## 3 - Linked List
Consists of a linked list datatype, and its related functions.

- [ft_lstnew()](./src/linked_list/ft_lstnew.c): Creates and retuns a new linked list node.
- [ft_lstadd_front()](./src/linked_list/ft_lstadd_front.c): Adds a node to the front of a linked list.
- [ft_lstadd_back()](./src/linked_list/ft_lstadd_back.c): Adds a node to the end of a linked list.
- [ft_lstsize()](./src/linked_list/ft_lstsize.c): Returns the size of a linked list.
- [ft_lstlast()](./src/linked_list/ft_lstlast.c): Returns the last node of a linked list.
- [ft_lstdelone()](./src/linked_list/ft_lstdelone.c): Deletes a node of a linked list using the del function.
- [ft_lstclear()](./src/linked_list/ft_lstclear.c): Clears a linked list from start to end.
- [ft_lstiter()](./src/linked_list/ft_lstiter.c): Iterates through a linked list and executes the 'f' function on all nodes.
- [ft_lstmap()](./src/linked_list/ft_lstmap.c): Duplicated a linked list and applies function 'f' on all the new nodes.

## 4 - Math
Consists of simple math functions.

- [ft_abs()](./src/math/ft_abs.c): Returns the absolute value of an integer.
- [ft_intlen()](./src/math/ft_intlen.c): Returns the number of digits in an integer.

# 5 - Memory
Consists of memory manipulation functions.


# Authors
Yaşam Ensar Demirkıran ([yademirk][yademirk-url])


[yademirk-url]: https://intra.42.fr/users/yademirk

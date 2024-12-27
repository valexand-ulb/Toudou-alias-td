# Toudou alias td

Simple todo list CLI written in C 

## Installation : 

- Clone repository
```shell
git clone https://github.com/valexand-ulb/Toudou-alias-td
cd Toudou-alias-td/
```

- CMAKE and MAKE

```shell
mkdir build && cd build
cmake ..
make .
```

- (Optional) cp executable to `/usr/bin`
## Usage

`td` has 4 command  :
- list : list existing todo
- add : add todo to todolist
- done : remove a todo from list
- time : modify time from a todo

### `td list`

List todo registered
```shell
td list             # default listing of 3 first todos by ids
td list 5           # list first 5 todos by ids
```

### `td add`

Add a todo to the todo list. Command has the following syntax
```shell
td add <content> <additional_time>
```

For example : 
```shell
td add "some thing to do" +8d
```

The additional time is in the form `{+,-, }[int]{h,d,w,m,y}` and is added from the current unix timestamp
```shell
1h      # + 1 hour
+2d     # + 2 days
-3w     # - 3 weeks
4m      # + 4 months
+5y     # + 5 years
```

### `td done`

Remove a todo from the todo list

```shell
td done 1   #remove todo of id 1
```

### `td time`
Modify time of a todo from the todo list. The additional time is added from the current todo time
Command has the following syntax
```shell
td time <todo_id> <additional_time>
```

For example :
```shell
td time 2 -6d
```
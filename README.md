# Final Course Project

This is a final course project made in *C++*. Each folder is an independent project, and they are the base for the more complex ones. My main page can be accessed [here](https://linux.ime.usp.br/~cjinshian/).

# Contents

This project contains generic tree-related algorithms. Since they were implemented as generic types, you can build each of them where their nodes hold comparable types like integers, floats, strings, chars etc.

*Note*: if you want to create a graph of your own custom classes, then you must implement comparison operators for your class, to ensure the API functions correctly. 

# Splay Tree

This is a generic splay tree class that uses a template parameter Key, which can be of any type. 

# Dynamic Forest with repeated ends

This is a dynamic forest based on splay trees, and its vertices are edges that contain repeated ends in the *Euler Tour*. For example, we can have vertices like *<u, u>*.

# Dynamic Forest with unique ends

This is a dynamic forest based on splay trees, and its vertices are edges that **DOES NOT** contain repeated ends in the *Euler Tour*. We can only have *<u, v>* vertices, where *u &ne; v*.

# Dynamic Graph

This is a dynamic graph based on dynamic forests with repeated ends. The dynamic graph accepts the following input:

```
n
v_1
v_2
.
.
.
v_n
```

where *n* is the number of vertices and v<sub>i</sub> (1 ≤ i ≤ n) is the value each vertex carries. 

Then you follow with the operations like:
```
1 a b
2 a b
3 a b
4
```

where: 

* **1 a b**: checks if the vertices a and b are connected;
* **2 a b**: links the vertices a and b, thus creating the edge <a, b>;
* **3 a b**: removes the edge <a, b>;
* **4**: prints out the forests (where each forest prints out their trees distinguished by their ID's) and the adjancency lists.

## How to test the dynamic graph

Once you have created a test file *t*, simply run:

```bash
make build
```

to compile the main *cpp* file, then run the following to see the output:

```bash
make run < t
```

# Decremental Minimum Spanning Forest

This is a partially dynamic graph that maintains Minimum Spanning Forests (MSFs), which means that the total weight of all edges that form an MSF is minimized compared to all other possible spanning forests. It is called partially because it only accepts connectivity and removal queries, which means that any edge addition query will be computed beforehand.  

The decremental MSF accepts the following input:

```
n
v_1
v_2
.
.
.
v_n

m
e_1
e_2
.
.
.
e_m
```

where *n* is the number of vertices, such that v<sub>i</sub> (1 ≤ i ≤ n) is the value each vertex carries, and *m* is the number of edges e<sub>j</sub> (1 ≤ j ≤ m) of the form 

```
v_1 v_2 w
```

where *w* is the weight of the edge that connects the vertices v<sub>1</sub> and v<sub>2</sub>.

After the input, you can follow with the operations like:

```
1 a b
2 a b
3
4
5
6
```

where: 

* **1 a b**: checks if the vertices a and b are connected;
* **2 a b**: removes the edge <a, b>;
* **3**: prints out the total weight of the minimum spanning forest;
* **4**: prints out the weighted edges of the minimum spanning forest;
* **5**: prints out the forests, where each forest prints out their trees distinguished by their ID's;
* **6**: prints out the adjancency lists.

## How to test the decremental MSF

Once you have created a test file *t*, simply run:

```bash
make build
```

to compile the main *cpp* file, then run the following to see the output:

```bash
make run < t
```



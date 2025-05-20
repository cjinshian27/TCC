# Final Course Project

This is a final course project made in *C++*. Each folder is an independent project, and they are the base for the more complex ones. My main page can be accessed [here](https://linux.ime.usp.br/~cjinshian/).

# Splay Tree

This is a generic splay tree class that uses a template parameter Key, which can be of any type. 

# Dynamic Forest with repeated ends

This is a dynamic forest based on splay trees, and its vertices are edges that contain repeated ends in the *Euler Tour*. For example, we can have vertices like *<u, u>*.

# Dynamic Forest with unique ends

This is a dynamic forest based on splay trees, and its vertices are edges that **DOES NOT** contain repeated ends in the *Euler Tour*. We can only have *<u, v>* vertices, where *u &ne; v*.

# Dynamic Graph

This is a dynamic graph based on dynamic forests with repeated ends. It is still under construction, so you may encounter bugs or segmentation faults if you test it now.

The dynamic graph accepts the following input:

```
n
x_1
x_2
.
.
.
x_n
```

where *n* is the number of vertices and x<sub>i</sub> (1 ≤ i ≤ n) is the value each vertex carries. Since it is a generic type, you can build a graph of comparable types like integers, floats, strings, chars etc.

*Note*: if you want to create a graph of non comparable classes, then you have to build *Overload Comparison Operators* for your class, in order to have the API working properly. 

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
* **3 a b**: remove the edge <a, b>;
* **4**: prints out the forests (where each forest prints out their trees distinguished by their ID's) and the adjancency lists.

## How to test the dynamic graph

Once you have created a test file, simply run:

```bash
make build
```

to compile the main *cpp* file, and then run:

```bash
make run < t
```
where *t* is the filename you have created.
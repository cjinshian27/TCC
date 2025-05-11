# Final Course Project

This is a final course project made in *C++*. Each folder is a independent project, and they are the base for the more complex ones. Splay Tree is used 
in both dynamic forest implementations, and dynamic graph uses the dynamic 
forest with repeated ends.

# Splay Tree

This is the generic splay tree class that uses a template parameter Key, which can be of any type. 

# Dynamic Forest with repeated ends

This is a dynamic forest based on splay trees, and its vertices are edges that contain repeated ends in the *Euler Tour*. For example, we can have vertices like *<u, u>*.

# Dynamic Forest with unique ends

This is a dynamic forest based on splay trees, and its vertices are edges that **DOES NOT** contain repeated ends in the *Euler Tour*. We can only have *<u, v>* vertices, where *u ≠ v*.

# Dynamic Graph

This is a dynamic graph based on dynamic forest with repeated ends. It is still under construction.
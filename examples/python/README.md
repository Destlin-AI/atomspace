# Python Examples

## Intro Remarks
The examples below are written in python. At the philosophical level,
this is just plain wrong. Atomese is not intended for human programmers,
it is intended for automated algorithms. This means that the syntax for
Atomese is was designed to make it easy for algorithms to create,
process and run it. The syntax is very easy to parse, manipulate,
process and rewrite: after all, Atomese is 'just; a (hyper-)graph.

This also means that its verbose, and often awkward for human
programmers. You can express anything in Atomese, but it is not
a human-freindly programming language, and was never meant to be.

Roughly speaking, programming in Atomese is like programming in
assembly: it can be done, and many humans specialize in and enjoy
assembly coding. However, most of the rest stick to high-level
languages.  The analogy here, though, is flawed: Atomese is for
machine manipulation; there isn't a high-level langauge built on
top of it, nor should there be. Conventional programming languages
already do an excellent job. Atomese provides graphs. Use it as
a graph processing system.  That's what it's for,

That said: you can't debug a complex graph processing algorithm
if you don't understand what the graph represents, and how it encodes
structure. The demos in this directory walk through some of the basics
of Atomese, using python as the embedding language.

Review the wiki page for more:
[Atomese](https://wiki.opencog.org/w/Atomese)

## Tutorial
Start a python3 shell in your favorite way.

* From the python prompt, the following will list the currently
  installed OpenCog python modules:
  ```
      help('opencog')
  ```

* The contents of a single module can be viewed by using `dir` function.
  For example,
  ```
      import opencog.atomspace
      print(dir(opencog.atomspace))
      print(dir(opencog.type_constructors))
  ```

* You can run the examples from your shell. For example,
  ```
      python3 storage_tutorial.py
  ```

## create_atoms.py
The most basic example of creating Atoms in an AtomSpace, and attaching
weights to them.

## storage_tutorial.py
A relatively simple all-in-one tutorial introducing basic concepts,
a practical example, and the use of the store-to-disk StorageNode API.

## vector_tutorial.py
A more complex example, showing how to perform queries, how to use the
query system to perform basic processng (counting, in this example) and
how to vectorize the results (so that vector data an be fed to GPU's.)

## create_atoms_lowlevel.py
For the bulk loading of an AtomSpace from python, direct access
to the AtomSpace is faster. The code is a little harder to read,
and exhibits less of the 'natural' Atomese syntax,

## nameserver_example.py
The AtomSpace includes a nameserver and class factory. These allow
Atom Types to be directly accessed. This is an advanced demo; very
few users will need to work with the nameserver.

## stop_go.py
An example of a "behavior tree".

## Mixing python and scheme
Atomese has both scheme and python bindings, and the two programming
langauges and styles can be freely intermixed. That is, you can call
scheme from python, and python from scheme, and everything "just works".

### scheme_sharing.py
A basic example of sharing an AtomSpace between python and scheme.

### scheme_timer.py
Simple measurement of the performance overhead of invoking the scheme
(guile) evaluator.

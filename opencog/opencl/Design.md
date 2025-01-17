Design Notes
------------
Notes about OpenCL interfaces and how they impact Atomese design.

* Vectors will need a corrsponding (private) `cl::Buffer` object
  (per vector).  The `cl::Buffer()` ctor requires a `cl::Context`.
  Some `cl::Buffer` ctors don't require a context; these all use
  the default context instead (seems like a bad idea, for us...)

* Alternative is to use SVM "Shared Virtual Memory", The ctors
  also need `cl::Context` and if not supplied, uses the default.

* The kernel itself needs only a `cl::Program` which holds the
  actual kernel code.

* Kernels are executed by placing them onto a `cl::CommandQueue`.
  This queue takes both a `cl::Context` and also a `cl::Device` 
  in it ctor. Kernels are exec async.

* Obtaining results from the exec needs a `cl::Event`, created when
  the kernel is enqueued.

Design alternatives:
* Use `GroundedProceedureNode`s to wrap kernels. Old-style, yucky.
  Why yucky? Because its stateless: besides the string name of the
  node, nothing is stored in the atom. There's no open/close phasing.
  Its just a function call. Maps poorly onto stateful I/O.

* Use `StorageNode`s to wrap kernels. This presents other issues.
  First, traditional `StorageNodes` were meant to send/recieve atoms,
  while here, we need to send/receive vectors represented as
  `FloatValue`s.  Maybe other kinds of vectors, but for now,
  `FloatValue`s.  The other problem is that the `StorageNode`
  API sits outside of Atomese proper, and uses instead a collection
  of calls `cog-open` and `cog-close` to manage connection state.
  Which is adequate for the intended use, but leaves something to be
  desired.

* Create something new, inspired by `BackingStore`. This removes some
  of the issues with StorageNodes, but leaves others in place. One
  is that, again, the API sits outside of Atmoese. The other is that
  this becomes yet another engineered solution. Of course, engineered
  solutions are inescapable, but... for example: the traditional unix
  open/close/read/write 4-tuple provides an abstraction that works very
  well for many I/O tasks: open/close to manage the channel, and
  read/write to use it. But the OpenCL interfaces do not map cleanly
  to open/close/read/write. They use a more compex structure.

The abstraction I'm getting is this: open near point, open far point,
select far-point message receipient, send message. That mapping would
have psuedocode like so:
```
   open_near_point() {
      Create cl::Context and retain pointer to it
   }

   open_far_point() {
      Using cl::Context from near point, and externally specified
      cl::Device, create a cl::CommandQueue. Retain pointer to the
      cl::CommandQueue. Its the commo handle.
   }

   select_recipient() {
      Create a cl::Event as the commo handle, retain pointe to it.
      Use the externally supplied cl::Program, retain pointer to it.
   }

   write() {
      Using the selected recipient, create a cl:Kernel.
      Call cl::Kernel::setArgs() in th kernel to establish the connetion.
      Call cl::CommandQueue::enqueueNDRangeKernel() to perform send
   )

   read() {
      Using the selected recipient, wait on cl::Event
   }
```

So this is more complex than open/close/read/write. There are three
choices:
* Codify the above as "sufficiently generic", claiming that e.g. CUDA
  would also fit into this model.
* Recognize the above as a cascade of opens(), each requiring the prior
  so that it resembles the peeling back of an onion.
* Recognize that the peeling-of-an-onion model is too 'linear', and that
  there is a network of interactions between the various `cl::` classes.
  That network is not a line, but a DAG. Encode the DAG as Atomese.
  That is, create an Atom that is more-or-less in 1-1 correspondence
  with the OpenCL classes. Comminication then requirs connecting the
  Atomese DAG in the same way that the OpenCL API expects the
  conections.

I like this third option. But how would it work, in practice?

Three choices for wiring diagrams:
* `RuleLink`
* `EvaluationLink`
* `Section`

Pros and cons:

### EvaluationLink
Olde-school. Uses  `PredicateNode` for the function name, and a list
of inputs, but no defined outputs. Can be given a TV to indicate
probability, but no clear-cut interpretation of teh function arguments.
Replaced by `EdgeLink` for performance/size.

### RuleLink
* Fairly compact.
* Distinct "input" and "output" areas.
* Rules are nameless (anonymous) with no built-in naming mechanism.
* Rules are explicitly heterosexual (everything is either an input, or
  an output, and must be one or the other.) This is problematic if
  inputs or outputs are to be multiplexed, or given non-directional
  (monosexual) conntions.
* No explicit connection/gluing semantics.

The good news about `RuleLink` is that it's fairly compact. The bad news

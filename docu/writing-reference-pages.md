# Tutorial: Writing Reference Pages | Cycling '74

Here I will cover all of the options available to you as you as an author of reference pages and also provide you with a style guide that will help with crafting the content you put into these pages.

## Reference Markup Tags and Attributes

First, let's begin with a review of all the available tags and attributes you may encounter in a reference page XML file.

### Top-level Elements

Here's a quick rundown of the top-level elements we use when formatting a refpage. You'll find examples of all of these elements that demonstrate their use in Part three of our Package authoring series

*   \<c74object\> : wrapper tag for a reference doc @name = the object name (required) @module = general object classification (one of 'max', 'msp', 'jit' or 'm4l') (required) @category = the object Category in the Object Explorer (required)
*   \<objarglist> \- The wrapper tag for object arguments
*   \<methodlist> \- wrapper tag for object methods
*   \<attributelist> \- wrapper tag for object attributes
*   \<examplelist> \- wrapper tag for refpage example pix
*   \<seealsolist> \- wrapper tag for See Also listings
*   \<digest> - digest wrapper (required for every object, method, attribute, argument, enum)
*   \<description> - description wrapper (required for every object; optional in all other cases)
*   \<discussion> - discussion wrapper (optional)

### 

Text Markup Used With \<digest>, \<description>, and \<discussion> Tags

\<o> \- a Max object (the object name goes between the tags). This creates an automatic link to that object's documentation. Under some circumstances, you might need to use the \<link> tag (as described below).

\<m> - any message sent in Max (not necessary in a message box -- essentially input and output from objects)

\<at> - a Max object attribute

\<ar> - a Max argument (object argument)

\<link> - a non-object documentation link, or a link to an object which requires special link text of some sort. For instance, "\<link name='live\_symbols' module='core' type='vignette'>here\</link>". Note the similarity to \<seealso> @name = the docs file name to be linked, without the extension (required) @type = the docs file type (one of 'refpage', 'tutorial' or 'vignette') (optional, 'refpage' is assumed) @module = the location of the file to be linked (optional, if a refpage \[the module can be derived\]). This value depends on the type of file and its location within the m5-docs directory. For instance, tutorials can be 'max', 'msp' or 'jit'. Vignettes can be 'core', 'intro', 'js' or 'thesaurus'.

\<br/> - a line break

\<a> - standard HTML anchor (only use if necessary for external links)

\<b> - standard HTML bold (available, but not recommended)

\<u> - standard HTML underline (available, but not recommended)

\<i> - standard HTML italics (available, but not recommended)

### Text Markups Used for Everything Else

*   \<objarg> - a Max object argument (in-box argument) @name = the argument name (required), a 1-3 word descriptor for the argument @type = the argument type ('int', 'float', 'symbol', 'atom') (required) @optional = optional flag (0/1). if missing, 0 is assumed

The object argument listing documentation can contain: \<digest> (required) \<description> (optional) \<discussion> (optional)

*   \<method> - a Max object method @name = the method name (e.g. "bang") (required) The method listing documentation can contain: \<arglist> - wrapper for object method args, if any, which contains: \<arg> - a method argument @name = the argument name (required), a 1-3 word descriptor for the argument @type = the argument type ('int', 'float', 'symbol', 'atom') (required) @optional = optional flag (0/1). if missing, 0 is assumed @units = units (one of 'time-value' \[can be replaced with appropriate ITM units description in the relevant places\], 'ms', 'hh:mm:ss', 'ticks', 'bars.beats.units', 'notevalues', 'hz', 'samples', 'db', 'pixels', 'quicktime' \[for jit.qt.objects which use QT time units for editing\]) (optional) \<digest> (required) \<description> (optional) \<discussion> (optional)
*   \<attribute> - a Max object attribute @name = the attribute name (e.g. "minimum") (required) @get = the attribute's getability (autocompleted, can be ignored) @set = the attribute's setability (autocompleted, can be ignored) @type = the attribute's type (autocompleted, can be ignored, unless it's inaccurate) @size = the attribute's size (autocompleted, can be ignored, unless it's inaccurate) The attribute documentation can contain:

\<digest> (required) \<description> (optional) \<discussion> (optional) \<attributelist> - attr attrs (optional) -- these can in almost all cases be ignored. HOWEVER, the attribute "enumvals" is special. It can contain: \<enumlist> \- wrapper for enumerator list, which contains: \<enum> - an enumerated value for an attribute @name = the enumerated value (e.g. "Numeric" or "Legacy" – this is automatically generated and won't need to be touched, unless the object changes). It can contain: \<digest> (required) \<description> (optional) \<discussion> (optional)

*   \<example> \- an image file showing the documented object at work @img = the image file to be displayed. if there is no example image, you can leave this blank. @caption = a caption for the image file
*   \<seealso> - see also... Similar to \<link>, but does not support special link text. Link text is auto-generated from the \<digest>of the document being linked. @name = the docs file name to be linked, without the extension (required) @type = the docs file type (one of 'refpage', 'tutorial' or 'vignette') (optional, 'refpage' is assumed) @module = the location of the file to be linked (optional, if a refpage \[the module can be derived\]). This value depends on the type of file and its location within the m5-docs directory. For instance, tutorials can be 'max', 'msp' or 'jit'. Vignettes can be 'core', 'intro', 'js' or 'thesaurus'.
*   \<misc> - a miscellaneous documentation section, for information or discussion which doesn't easily fit anywhere else. These may be multiple (e.g. \<misc name="foo"> ... \</misc> \<misc name="bar"> ... \</misc>). @name = the name of the documentation section, to be displayed as a header (required)          The misc documentation can contain:

\<entry> - a subsection of the misc documentation (required) @name = the entry name (required)

It can contain: \<digest> (required) \<description> (optional) \<discussion> (optional)

### Object digest: \<digest>

*   Should be 3-5 words that succinctly describe the object’s function. For example, “Convert numbers into birds” rather than “A process which takes a number and turns it into a bird”.
*   Should use active verbs (as though it were prefaced with an implicit “Use this object/method to…”. And specifically not with an implicit “This object/message…”, which results in a different verb conjugation. We want it to say “Convert” not “Converts”).
*   The active verb should describe the desired object/method action rather than operating the patch or max as an application .
*   Remove any lingering periods at the end of the digest.

### Object descriptions\<description>

If it's not necessary leave the object description empty. If necessary:

*   It should always include a restatement of the digest.
*   It should strive to be no more than 3 (complete) sentences long. Can be shorter when 1 or 2 sentences suffice and need no further clarification. 
    *   What the object can be used for
    *   Why you might want to do this (or a clarifying comment)
    *   How this object can be differentiated from other similar objects, a further refinement, or a clarifying comment. This can encourage discoverability of other similar or related features, while distinguishing this object.
*   Should be progressively less important (“newspaper inverted pyramid”)

.css-1ygpvhy{border-left-width:6px;border-left-style:solid;border-left-color:var(--theme-ui-colors-light30);background-color:rgba(0,0,0, 0.05);padding-top:16px;padding-bottom:16px;padding-right:8px;padding-left:32px;margin-top:32px;margin-bottom:32px;margin-left:0;margin-right:0;}@media screen and (min-width: 768px){.css-1ygpvhy{margin-left:32px;margin-right:32px;}}

> Bad: "The bgcolor object's functionality is equivalent to the brgb message of the thispatcher object..." Good: “Use the bgcolor object to set the patcher’s background color. You can also set the background color in the patcher inspector or with the brgb message to thispatcher.”

*   Side topics or more detailed discussion should go in the \<discussion> tag.
*   Objects should be referred to in the \<o> tag in the body of the ref.

### Arguments

*   All arguments should be separated into individual argument elements—i.e. no single “list” argument with several argument names.
*   Argument names should be short, clear, and non-hyphenated whenever possible. However for truly extreme cases where a separator is required for an argument name, hyphens should be used instead of underscores, [InterCaps](https://app.assembla.com/wiki/show/max/InterCaps), etc.
*   Argument types ideally should not be a list of types like “int float symbol”. They should strive to be one type (int, float, number, symbol, atom). This could be assisted by a reference pass script. For “int float” use the word “number”.
*   Object argument lists have both digests and descriptions. Digest is used in all cases, and should fit on one line. If necessary, use a description, which should always have a restatement of the digest, as per object description. Note that this based on showing argument info in autocompletion where it is “argument - digest” shown in a list plus an extra panel at the bottom that can show a description if present.
*   The \<arg> tag no longer has an id attribute. It has been removed from the edits and will be ignored by the parser. Args need to be listed in the correct order in the refpage.
*   Arguments can be referred to in the body of the ref with the \<ar> tag. It won't be specifically marked up at this point, but it will be there to use. Example arg: \<arg name="delay" optional="0" type="int" units="ms" />

Presentation Notes: Data types can remain the same in the XML reference (“int”, “float”, “symbol”, “atom”), however for the display, these may be mapped to something more user friendly like: “integer”, “float” (“number” when both are acceptable), “symbol”, or “any”. No argument should be of type “list”.

### Messages

Message digests (See object digests): Message digests hould ideally be 1-6 words, no more than 10, and fit on a single line.

Digests for messages should not need to restate arguments but could discuss implicitly--e.g. “Sets input range” rather than “Sets range to lie between the first argument and the second argument”.

You should not put long descriptions as duplicates in the digest field. For messages that could be used in a variety of inlets, the digest can state "Function depends on inlet" to identify that the message has multiple functions which cannot be paraphrased together concisely.

Message descriptions: 

The message description follows similar style as object descriptions. Message descriptions like argument descriptions are now optional (just like arguments), and only need to be present if there is need for further discussion. For messages that can be used in a variety of inlets, separate each inlet usage with a blank line.

Message arguments:

Ideally, there is no need for digest or description. Instead, you should use a clear and concise argument name.

If there is no need to describe or name a message argument, use the word “input” and it will be ignored (e.g., by message box autocompletion).

Examples include int and float method, where the description of the message will describe the argument.

In those places where a refpage contains a reference to another method - such as: "Performs the same function as the \<m>float\</m> message. See above.", you should use "See the \<m>(message name>\</m> listing" Here is an example message:

\<method name='midinote'>
\<arglist>
    \<arg name='pitch' optional='0' id='0' type='int'/>
    \<arg name='velocity' optional='0' id='1' type='int'/>
\</arglist>

### Attributes

Attribute digests:

Attributes should be described as nouns where possible, without additional pronouns or wordiness. 

> Bad: “transport: This attribute names a master time transport.” Good: “transport: Master time transport”

Flags should be described as active verbs like methods, where the verb describes the “On/True” setting. Avoid the verb “set” wherever possible.

> Bad: “defer: Turn this on to send output in the low-priority queue.” Good: “defer: Output at low-priority.” Or “defer: Defer output to low priority”

Attribute Descriptions: 

Ideally three should be no need for a description. Instead, use a clear and concise digest. Descriptions should be used when there’s a large amount of information necessary to share.

In those places where a refpage contains a reference to another attribute, the \<at> tag can be used, like: "if the object's \<at>initial\</at> attribute has been set".

Here is an example attribute:

\<attribute name='ambient' get='1' size='4' set='1' type='float'> 

### Inlets/Outlets

The \<inletlist> and \<outletlist>elements (and their children) should be ignored in the ref files at the time of this writing. Instead, you should describe inlet input in each method, as it is generally done in the current ref. To describe output, use the \<misc name='Output'> tag, as is currently done. If needed, edit the \<description> tag.

Final Thoughts


------------------

Writing good reference documentation can be a daunting task. There are dozens of decisions to make and it isn't always clear how to word things for maximum effect. I hope this peak inside of our guiding approaches will make the process easier as you craft your own additions to the Max ecosystem.

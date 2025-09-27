---

marp: true
enableHtml: true
__theme: gaia
class: invert
paginate: true
_paginate: false
math: mathjax



---

<!-- <script>

// app.use(cors({
//   origin: 'https://tikzjax.com'
// }));

</script>

<link rel="stylesheet" type="text/css" href="https://tikzjax.com/v1/fonts.css">
<script src="https://tikzjax.com/v1/tikzjax.js"></script> -->


<style>

.picture {
  position: absolute; 
  left: 50;
  bottom: 0; 
 }

#hp  {
float: right;    
 margin: 0 0 0 15px;
 vertical-align:middle;
}

.box {
   display: flex;
   align-items:center;
   text-align:right;
   justify-content: flex-end
}

    .container {
    display: flex;
    justify-content: center;
    }
    .byte {
        display: inline-block;
        text-align: center;
    }
    .bit {
        width: 50px;
        height: 50px;
        border: 1px solid black;
        display: inline-block;
        text-align: center;
        line-height: 50px;
        margin-right: 2px;
    }
    .bit1 {
        color: black;
        background-color: #60ff60;
    }
    .bit0 {
        color: white;
        background-color: #c00000;
    }
    .bitc1 {
        color: black;
        width: 130px;
        background-color: #40c040;
    }
    .bitc0 {
        color: white;
        width: 130px;
        background-color: #903030;
    }
    .bitg {
        color: black;
        background-color: #808080;
    }
    .bitcg {
        width: 130px;
        background-color: #808080;
    }
    .segct_rest{}
    .observed{}

</style>

![bg](pictures/title_page_2.png)

<!-- <img class="picture" style="width:360px; float:right" src="pictures/luna_rolf_2.png"/> -->

<!-- <div class="box" >
<img class="box" style="width:360px; float:right" src="pictures/luna_rolf_2.png"/>
</div> -->

<!-- ![width:400px center](pictures/luna_rolf_2.png) -->

<!-- <div class="box" >
<span style='color:white; font-size: 150%;font-weight: 500'>me!</span><img src="pictures/luna_rolf_2.png" style=" width:560px" id="hp"/>
</div> -->

<!-- <img src="pictures/Luna_Meersau.png" style=" width:400px; float:right" /><div style="clear:both; text-align:right"><h2>come and hop with me!</h2></div> -->



---
<!--header: 'outline'-->

# Outline

- Using bits for Boolean options

* Builtin bitwise operators

* The Complement-Problem

* Boost.Flags Library

* Other Approaches 

---
<!--header: 'Using bits for Boolean options'-->

# Using bits for Boolean options

---
<!--header: 'Using bits for Boolean options'-->

## Easy to define, e.g. as macros, integer constants or enumerators
* <pre><code><div style="font-size:100%"><span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">OPTION_ONE</span>&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x1</span>
  <span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">OPTION_TWO</span>&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x2</span>
  <span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">OPTION_THREE</span>&nbsp;<span style="color:#b5cea8;">0x4</span></div></code></pre>

* <pre><code><div style="font-size:100%"><span style="color:#569cd6;">static</span>&nbsp;<span style="color:#569cd6;">constexpr</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#c8c8c8;">option_one</span>&nbsp;&nbsp;&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x1</span><span style="color:#b4b4b4;">;</span>
  <span style="color:#569cd6;">static</span>&nbsp;<span style="color:#569cd6;">constexpr</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#c8c8c8;">option_two</span>&nbsp;&nbsp;&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x2</span><span style="color:#b4b4b4;">;</span>
  <span style="color:#569cd6;">static</span>&nbsp;<span style="color:#569cd6;">constexpr</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#c8c8c8;">option_three</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x4</span><span style="color:#b4b4b4;">;</span></div></code></pre>

* <pre><code><div style="font-size:100%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">options_enum</span>&nbsp;<span style="color:#b4b4b4;">{</span>
  &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">option_one  </span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x1</span><span style="color:#b4b4b4;">,</span>
  &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">option_two  </span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x2</span><span style="color:#b4b4b4;">,</span>
  &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">option_three</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x4</span>
  <span style="color:#b4b4b4;">};</span></div></code></pre>
---

<!--header: 'Using bits for Boolean options'-->

## Compact representation in memory

  * `int` can hold up to 16 / 32 different Boolean options 
    <div class="container">
    <div class="byte">
    <div class="bit bit1">b<sub>0</sub></div>
    <div class="bit bit0">b<sub>1</sub></div>
    <div class="bit bit0">b<sub>2</sub></div>
    <div class="bit bit1">b<sub>3</sub></div>
    <div class="bit bit1">b<sub>4</sub></div>
    ...
    <div class="bit bit0">b<sub>28</sub></div>
    <div class="bit bit1">b<sub>29</sub></div>
    <div class="bit bit0">b<sub>30</sub></div>
    <div class="bit bit1">b<sub>31</sub></div>
    <!-- <div class="bit bitc1 ">b<sub>4</sub></div>
    <span class="">...</span> 
    <div class="bit bitc1 ">b<sub>31</sub></div> -->
    </div>
    </div><br/>

  * Simple & efficient bulk transfer in interfaces

  * Compressed version of a `std::array<bool, 32>`

---

<!--header: 'Using bits for Boolean options'-->

##  Enforces the usage of names / prevent bool parameters

* Declaration
  <pre><code><div style="font-size:100%"><span style="color:#569cd6;">void</span>&nbsp;<span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">(</span><span style="color:#569cd6;">bool</span>&nbsp;<span style="color:#9a9a9a;">use_opt1</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">bool</span>&nbsp;<span style="color:#9a9a9a;">use_opt2</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">bool</span>&nbsp;<span style="color:#9a9a9a;">use_opt3</span><span style="color:#b4b4b4;">);
  </span>
  <span style="color:#569cd6;">void</span>&nbsp;<span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">(</span><span style="color:#4ec9b0;">options_enum</span>&nbsp;<span style="color:#9a9a9a;">options</span><span style="color:#b4b4b4;">);</span></div></code></pre><br/>

* Call site
  <pre><code><div style="font-size:100%"><span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">(</span><span style="color:#569cd6;">true</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">false</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">true</span><span style="color:#b4b4b4;">);
  </span>
  <span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">(</span><span style="color:#b8d7a3;">option_one</span>&nbsp;<span style="color:#b4b4b4;">|</span>&nbsp;<span style="color:#b8d7a3;">option_three</span><span style="color:#b4b4b4;">);</span></div></code></pre>



---

<!--header: 'Using bits for Boolean options'-->

## Builtin support through bitwise operators

* <pre><code><div style="font-size:100%">bitwise AND: <span style="color:#569cd6;">int</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">&amp;(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span>         <span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">&amp;=(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span></div></code></pre>
* <pre><code><div style="font-size:100%">bitwise OR:  <span style="color:#569cd6;">int</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">|(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span>         <span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">|=(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span></div></code></pre>
* <pre><code><div style="font-size:100%">bitwise XOR: <span style="color:#569cd6;">int</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">^(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span>         <span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">^=(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&amp;</span><span style="color:#b4b4b4;">,</span>&nbsp;<span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span></div></code></pre>
* <pre><code><div style="font-size:100%">bitwise Negation: <span style="color:#569cd6;">int</span>&nbsp;<span style="color:#569cd6;">operator</span><span style="color:#b4b4b4;">~(</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">)</span></div></code></pre>

---


# Everything good!

---


# Everything good! - Really?


<!-- ---

header: 'Using bits for Boolean options'

## Why do we use the bits of integer types for Boolean options?

* Easy to define, e.g. as macros, integer constants or enumerators

* Compact representation in memory

* Simple & Efficient bulk transfer in interfaces
  - `unsigned int` can hold up to 16 / 32 different Boolean options

* Enforces the usage of names / prevent bool parameters in interfaces
  >    "Boolean arguments loudly declare that the function does more than one thing. They are confusing and should be eliminated."
  > C. Martin (Uncle Bob), Clean Code

* Language support for setting and querying through bitwise operators -->

<!-- --- -->
<!--header: 'Using bits for Boolean options'-->

<!-- # Bitwise operators in action ... -->

  <!-- <ul style="list-style: none;padding-left: 0;">
    <li data-marpit-fragment="1">
<pre><code><div style="font-size:80%"><span style="color:#57a64a;">/*&nbsp;Listbox&nbsp;Styles&nbsp;*/</span>
<span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">LBS_NOTIFY</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x0001L</span>
<span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">LBS_SORT</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x0002L</span>
<span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">LBS_NOREDRAW</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x0004L</span>
<span style="color:#9b9b9b;">#define</span>&nbsp;<span style="color:#beb7ff;">LBS_MULTIPLESEL</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b5cea8;">0x0008L</span></div></code></pre>   </li>
    <li data-marpit-fragment="2">
<pre><code><div style="font-size:100%"><span style="color:#57a64a;">//&nbsp;basic&nbsp;list-box&nbsp;options:&nbsp;sorting,&nbsp;notification</span>
<span style="color:#569cd6;">unsigned</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#c8c8c8;">opts</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#beb7ff;">LBS_SORT</span>&nbsp;<span style="color:#b4b4b4;">|</span>&nbsp;<span style="color:#beb7ff;">LBS_NOTIFY</span><span style="color:#b4b4b4;">;</span>
&nbsp;
<span style="color:#57a64a;">//&nbsp;add&nbsp;multi-selection</span>
<span style="color:#c8c8c8;">opts</span>&nbsp;<span style="color:#b4b4b4;">|=</span>&nbsp;<span style="color:#beb7ff;">LBS_MULTIPLESEL</span><span style="color:#b4b4b4;">;</span>
&nbsp;
<span style="color:#57a64a;">//&nbsp;remove&nbsp;sorting</span>
<span style="color:#c8c8c8;">opts</span>&nbsp;<span style="color:#b4b4b4;">&amp;=</span>&nbsp;<span style="color:#b4b4b4;">~</span><span style="color:#beb7ff;">LBS_SORT</span><span style="color:#b4b4b4;">;</span>
&nbsp;
<span style="color:#57a64a;">//&nbsp;notifications&nbsp;wanted?</span>
<span style="color:#d8a0df;">if</span>&nbsp;<span style="color:#b4b4b4;">(</span>opts&nbsp;<span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#beb7ff;">LBS_NOTIFY</span><span style="color:#b4b4b4;">)</span>&nbsp;<span style="color:#b4b4b4;">{</span>&nbsp;<span style="color:#b4b4b4;">...</span>&nbsp;<span style="color:#b4b4b4;">}</span>
</div></code></pre>
</li>
</ul> -->
 


<!--

The Return Type may depend on the number of arguments

-->
---
<!--header: 'Using bits for Boolean options'-->

# Bitwise operators in action ...

* integer types
  - all operators supported
  - no type-safety at all

* unscoped enums
  - all except assignment operators supported by implicit integer promotion
  - no type-safety for bitwise ops, `static_cast` to enum required

* scoped enums
  - no operator supported
  - `static_cast` to underlying and back to enum required


---
<!--header: 'Don&apos;t lose your head!'-->

# A short fairy-tale

* <h2>The Fun in this talk</h2>

---
<!--header: 'Don&apos;t lose your head!'-->

# Once upon a time...

---
<!--header: 'Don&apos;t lose your head!'-->

![width:800px center](pictures/palace.jpg)

<div align="right">
<sup style="font-size:60%;color:#505050">ht<span>tps://de.freepik.com/</span></sup>
</div>

---
<!--header: 'Don&apos;t lose your head!'-->

![height:600px center](pictures/Queen_of_Hearts_KHREC.webp)

<div align="right">
<sup style="font-size:60%;color:#505050">ht<span>tps://disney.fandom.com/</span></sup>
</div>

---
<!--header: 'Don&apos;t lose your head!'-->

![height:500px center](pictures/Iron_Rule.jpg)

<div align="right">
<sup style="font-size:60%;color:#505050">ht<span>tps://upload.wikimedia.org/wikipedia/commons/c/c4/Iron_Rule.jpg</span></sup>
</div>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:112%;visibility: hidden">
Proclamation: Lorem ipsum dolor sit amet, <i><u> The Queen</u></i>!<br/>
Proclamation: At vero eos et accusam, <i><u> The Queen</u></i>!<br/>
Proclamation: Stet clita kasd gubergren, <i><u> The Queen</u></i>!<br/>
Proclamation: Duis autem vel eum, <i><u> The Queen</u></i>!<br/>
Proclamation: Nam liber tempor cum soluta, <i><u> The Queen</u></i>!<br/>
Proclamation: Ut wisi enim ad minim veniam, <i><u> The Queen</u></i>!
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:112%">
Proclamation: Lorem ipsum dolor sit amet, <i><u> The Queen</u></i>!<br/>
Proclamation: At vero eos et accusam, <i><u> The Queen</u></i>!<br/>
Proclamation: Stet clita kasd gubergren, <i><u> The Queen</u></i>!<br/>
Proclamation: Duis autem vel eum, <i><u> The Queen</u></i>!<br/>
Proclamation: Nam liber tempor cum soluta, <i><u> The Queen</u></i>!<br/>
Proclamation: Ut wisi enim ad minim veniam, <i><u> The Queen</u></i>!
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:150%">
Proclamation:

All proclamations shall be encoded in C++,<br/> <i><u> The Queen</u></i>! 
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---

<pre><code><div style="font-size:140%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">font_styles</span>&nbsp;<span style="color:#b4b4b4;">:</span>&nbsp;<span style="color:#569cd6;">unsigned</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">bold</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">1</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">italic</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">2</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">underline</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">4</span>
<span style="color:#b4b4b4;">};</span>




</div></code></pre>



---

<pre><code><div style="font-size:140%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">font_styles</span>&nbsp;<span style="color:#b4b4b4;">:</span>&nbsp;<span style="color:#569cd6;">unsigned</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">bold</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">1</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">italic</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">2</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">underline</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">4</span>
<span style="color:#b4b4b4;">};</span>
<span style="color:#57a64a;">
//&nbsp;special&nbsp;style&nbsp;for&nbsp;the&nbsp;Queen&#39;s&nbsp;name</span>
<span style="color:#569cd6;">static</span>&nbsp;<span style="color:#569cd6;">constexpr</span>&nbsp;<span style="color:#569cd6;">auto</span>&nbsp;<span style="color:#c8c8c8;">queen_of_hearts_name</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b8d7a3;">italic</span>&nbsp;<span style="color:#b4b4b4;">|</span>&nbsp;
                                             <span style="color:#b8d7a3;">underline</span><span style="color:#b4b4b4;">;</span></div></code></pre>



---

<pre><code><div style="font-size:97%"><span style="color:#c8c8c8;">queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name
queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name</span>
</div></code></pre>


![height:400px center](pictures/programmer_tired_2.png)

<pre><code><div style="font-size:97%"><span style="color:#c8c8c8;">queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name
queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name</span>
</div></code></pre>

---

<pre><code><div style="font-size:97%"><span style="color:#c8c8c8;">queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name
queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name</span>
</div></code></pre>

![height:400px center](pictures/programmer_tired_lightbulb.png)

<pre><code><div style="font-size:97%"><span style="color:#c8c8c8;">queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name
queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name queen_of_hearts_name</span>
</div></code></pre>

---

<ul style="list-style: none;padding-left: 0;">
    <li>
<pre><code><div style="font-size:200%"><span style="color:#c8c8c8;">          queen_of_hearts_name           </span>
</div></code></pre>
</li>
    <li data-marpit-fragment="1">
<pre><code><div style="font-size:200%"><span style="color:#c8c8c8;">                  --></span>
</div></code></pre>
</li>
    <li data-marpit-fragment="2">
<pre><code><div style="font-size:200%"><span style="color:#c8c8c8;">                 ~bold</span>
</div></code></pre>
</li>
</ul>

---

<!--header: 'Don&apos;t lose your head!'-->

![height:600px center](pictures/happy_programmer.jpeg)

<div align="right">
<sup style="font-size:60%;color:#505050"><span>Microsoft Copilot</span></sup>
</div>


---

<!--header: 'Don&apos;t lose your head!'-->

![height:600px center](pictures/Alice.webp)

<div align="right">
<sup style="font-size:60%;color:#505050">ht<span>tps://disney.fandom.com/</span></sup>
</div>


---

<!--header: 'Don&apos;t lose your head!'-->

<pre><code><div style="font-size:140%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">font_styles</span>&nbsp;<span style="color:#b4b4b4;">:</span>&nbsp;<span style="color:#569cd6;">unsigned</span>&nbsp;<span style="color:#569cd6;">int</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">bold</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">1</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">italic</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">2</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">underline</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">4</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">strikeout</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">8</span>&nbsp;&nbsp;&nbsp;<span style="color:#57a64a;">//&nbsp;for&nbsp;Alice&#39;s&nbsp;name</span>
<span style="color:#b4b4b4;">};</span></div></code></pre>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:150%">
Proclamation:

<span style="visibility:hidden"> All persons whose names are stricken out are enemies of the kingdom,</span><br/> &nbsp;  
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:150%">
Proclamation:

All persons whose name is stricken out are enemies of the kingdom,<br/> &nbsp;  
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---
<!--header: 'Don&apos;t lose your head!'-->

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<td class="hop_experts_td" >
<div style="font-family:'Times New Roman'">
<center><h1 style="font-size:250%"> The Dayly Queen </h1></center>
<hr/>


<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">

<td class="hop_experts_td" style="width:250px">

![height:300px center](pictures/Queen_of_Hearts_KHREC.webp)

</td>

<td class="hop_experts_td" >

<span style="font-size:150%">
Proclamation:

All persons whose name is stricken out are enemies of the kingdom,<br/> <s><i><u> The Queen</u></i></s>! 
</span>
</td>

</tr>
</table>
</div>



</div>
</td>
</table>

---

<!--header: 'Don&apos;t lose your head!'-->

![height:600px center](pictures/programmer_running_away.jpeg)

<div align="right">
<sup style="font-size:60%;color:#505050"><span>Microsoft Copilot</span></sup>
</div>


---

<!--header: 'Don&apos;t lose your head!'-->

![width:1200px center](pictures/callout_off_with_his_head_queen_2.png)

<div align="right">
<sup style="font-size:60%;color:#505050">ht<span>tps://disney.fandom.com/</span></sup>
</div>

---
<!--header: 'Don&apos;t lose your head!'-->
<center>
<h1 style="font-size:500%"> The End </h1>
</center>

---
<!--header: 'Don&apos;t lose your head!'-->

# Moral:

<div style="display: flex;justify-content: center;">
<pre><code><div style="font-size:150%"><span style="color:#c8c8c8;">queen_of_hearts_name</span>&nbsp;<span style="color:#b4b4b4;">!=</span>&nbsp;<span style="color:#b4b4b4;">~</span><span style="color:#b8d7a3;">bold</span></div></code></pre>
</div>

* Syntactical
  <pre><code><div style="font-size:100%"><span style="color:#c8c8c8;">std</span><span style="color:#b4b4b4;">::</span><span style="color:#dcdcaa;">to_underlying</span><span style="color:#b4b4b4;">(</span><span style="color:#c8c8c8;">queen_of_hearts_name</span><span style="color:#b4b4b4;">)</span>&nbsp;<span style="color:#b4b4b4;">!=</span>&nbsp;<span style="color:#c8c8c8;">std</span><span style="color:#b4b4b4;">::</span><span style="color:#dcdcaa;">to_underlying</span><span style="color:#b4b4b4;">(~</span><span style="color:#b8d7a3;">bold</span><span style="color:#b4b4b4;">)</span></div></code></pre>

* Semantical
  * `~bold` is not a set of font-modifications
  * Excludes boldness from a given set of flags
  

---
<!--header: 'Complement'-->



<div class="container bits_top">
<div class="byte">
    <div class="bit bit1">b</div>
    <div class="bit bit1">i</div>
    <div class="bit bit1">u</div>
    <div class="bit bit1">s</div>
    <div class="bit bitc1 ">b<sub>4</sub> ... b<sub>31</sub></div>
    <!-- <div class="bit bitc1 ">b<sub>4</sub></div>
<span class="">...</span> 
<div class="bit bitc1 ">b<sub>31</sub></div> -->
</div>
</div>
<br/>
<br/>
<div class="container bits_top"><code>disjuction (|)</code></div>
<!-- <br/> -->
<div class="number_flags"><code><span>dims: #flags</span>     <span class="number_flags_ex">&nbsp;&nbsp;   + 1</span></code></div>
<br/>
<div class="container">
<div class="byte bits_lhs">
    <div class="bit bit0">b</div>
    <div class="bit bit1">i</div>
    <div class="bit bit1">u</div>
    <div class="bit bit0 strikeout">s</div>
    <div class="bit bitc0 sect_rest">b<sub>4</sub> ... b<sub>31</sub></div>
    <!-- <div class="bit bitc0 sect_rest">b<sub>4</sub></div>
<span class="sect_rest">...</span>
<div class="bit bitc0 sect_rest">b<sub>31</sub></div>
</div> -->
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <code class="op_cpmpl"><--  ~  --></code>  &nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp; 
<div class="byte bits_rhs">
    <div class="bit bit1">b</div>
    <div class="bit bit0">i</div>
    <div class="bit bit0">u</div>
    <div class="bit bit1 strikeout_rhs">s</div>
    <div class="bit bitc1 sect_rest">b<sub>4</sub> ... b<sub>31</sub></div>
    <!-- <div class="bit bitc1 sect_rest">b<sub>4</sub></div>
<span class="sect_rest">...</span>
<div class="bit bitc1 sect_rest">b<sub>31</sub></div>
</div> -->
</div>
<br/><br/>
<div>
<div class="container" style="justify-content: left;"><code class="bits_lhs">italic | underline</code><code style="visibility:hidden">                               </code><code class="bits_rhs strikeout_rhs2" style="justify-content: right;"><span>!= </span></code><code class="bits_rhs NOPE_strikeout_rhs2__not" style="justify-content: right;"><span>bold</span></code></div>
<br/>
<div class="container bits_bottom"><code>conjuction (&)</code></div>
<br/>
<br/>
<div class="container bits_bottom">
<div class="byte">
    <div class="bit bit0">b</div>
    <div class="bit bit0">i</div>
    <div class="bit bit0">u</div>
    <div class="bit bit0">s</div>
    <div class="bit bitc0">b<sub>4</sub> ... b<sub>31</sub></div>
    <!-- <div class="bit bitc0 ">b<sub>4</sub></div>
<span class="">...</span>
<div class="bit bitc0 ">b<sub>31</sub></div>
</div> -->
</div>


<div class="observed" id="bits_lhs" data-marpit-fragment="1"> </div>
<div class="observed" id="bits_rhs" data-marpit-fragment="1"> </div>
<div class="observed" id="op_cpmpl" data-marpit-fragment="1"> </div>
<div class="observed" id="strikeout" data-marpit-fragment="1"> </div>
<div class="observed" id="strikeout_rhs" data-marpit-fragment="1"> </div>
<div class="observed" id="strikeout_rhs2" data-marpit-fragment="1"> </div>
<div class="observed" id="sect_rest" data-marpit-fragment="1"> </div>
<div class="observed" id="number_flags" data-marpit-fragment="1"> </div>
<div class="observed" id="number_flags_ex" data-marpit-fragment="1"> </div>
<div class="observed" id="bits_top" data-marpit-fragment="1"> </div>
<div class="observed" id="bits_bottom" data-marpit-fragment="1"> </div>






<script>

var observer = new MutationObserver(function(mutations) {
  mutations.forEach(function(mutation) {
    if (mutation.type === "attributes") {

      // Example of accessing the element for which 
      // event was triggered
//      mutation.target.textContent = "Attribute of the element changed";
    console.log("attrib changed");

    var invis = mutation.target.getAttribute("data-bespoke-marp-fragment") === 'inactive';     
    if (mutation.target.getAttribute("force-visibility") === 'on') {
        invis = false;
    } else if (mutation.target.getAttribute("force-visibility") === 'off') {
        invis = true;
    }      
    console.log(mutation.target.id);
    console.log(mutation.target);
    console.log(invis);

    var deps = document.getElementsByClassName(mutation.target.id + '__not');
    console.log(deps);
    for(var elem of deps){
        if(!invis){
        elem.style.visibility = 'hidden';
        }
        //  else {
        // elem.style.visibility = 'visible';
        // }
    }

    var deps = document.getElementsByClassName(mutation.target.id);
    console.log(deps);
    for(var elem of deps){
        if(invis){
        elem.style.visibility = 'hidden';
        } else {
        elem.style.visibility = 'visible';
        }
    }

    }
    
  });
});

// var element = document.getElementsByClassName('observed');
// console.log(element);

// observer.observe(element, {
//   attributes: true //configure it to listen to attribute changes
// });


var observed = document.getElementsByClassName('observed');
    console.log("add attrib observer");
   console.log(observed);
 for(var element of observed){
    observer.observe(element, {
    attributes: true //configure it to listen to attribute changes
    });
}




</script> 


---

<div class="container">
<div class="byte bits_lhs">
    <div class="bit bit0">b</div>
    <div class="bit bit1">i</div>
    <div class="bit bit1">u</div>
    <div class="bit bit0">s</div>
    <div class="bit bitc0">b<sub>4</sub> ... b<sub>31</sub></div>
</div>
</div>

<code>#flags: 4</code>
<code>#bit underlying type U: 32</code>


* Underlying type $U$ with `~, &, |`: _(bitcount of $U$)_-dim Boolean algebra 

* <code>flags</code> with `&, |` form a substructure $F$ with dimension `#flags`
  * in general *is not closed under bitwise negation `~`*.

* Closure $\overline{F}$ wrt. $\sim$
  * dimension `#flags + 1`, Boolean subalgebra of $U$

---

<div class="container">
<div class="byte bits_lhs">
    <div class="bit bit0">b</div>
    <div class="bit bit1">i</div>
    <div class="bit bit1">u</div>
    <div class="bit bit0">s</div>
    <div class="bit bitc0">b<sub>4</sub> ... b<sub>31</sub></div>
</div>
</div>
<br/>

- Elements of  $\overline{F} \setminus F$ :  negative flag-masks 
  <div class="container">
  <div class="byte bits_lhs">
    <div class="bit bitg">b<sub>0</sub></div>
    <div class="bit bitg">b<sub>1</sub></div>
    <div class="bit bitg">b<sub>2</sub></div>
    <div class="bit bitg">b<sub>3</sub></div>
    <div class="bit bitc1">b<sub>4</sub> ... b<sub>31</sub></div>
  </div>
  </div>
  
  * put into different Type  

* For given enumeration `E`
  - Type for flags: `E` 
  - Type for negative flag-masks: `complement<E>`

* Meta-level: type-set `{E, complement<E>}` with `∼`, `&` and `∣` is a Boolean algebra 

---

<!--header: 'Complement & underlying type'-->

<pre><code><div style="font-size:130%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">flag_0</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x1</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">flag_1</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x2</span>
<span style="color:#b4b4b4;">};</span>
<span style="color:#569cd6;">inline</span>&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#b4b4b4;">operator</span><span style="color:#b4b4b4;">~</span>&nbsp;<span style="color:#b4b4b4;">(</span><span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9a9a9a;">x</span><span style="color:#b4b4b4;">)</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#d8a0df;">return</span>&nbsp;<span style="color:#569cd6;">static_cast</span><span style="color:#b4b4b4;">&lt;</span><span style="color:#4ec9b0;">flags</span><span style="color:#b4b4b4;">&gt;(~</span><span style="color:#569cd6;">static_cast</span><span style="color:#b4b4b4;">&lt;</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&gt;(</span><span style="color:#9a9a9a;">x</span><span style="color:#b4b4b4;">));</span>
<span style="color:#b4b4b4;">}</span>
<span style="color:#57a64a;">//&nbsp;other&nbsp;operators&nbsp;...</span>
<span style="color:#569cd6;">void</span>&nbsp;<span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">()</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b8d7a3;">flag_0</span>&nbsp;<span style="color:#b4b4b4;">|</span>&nbsp;<span style="color:#b8d7a3;">flag_1</span><span style="color:#b4b4b4;">;</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9cdcfe;">mask</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b4b4b4;">~</span><span style="color:#b8d7a3;">flag_1</span><span style="color:#b4b4b4;">;</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#9cdcfe;">mask</span><span style="color:#b4b4b4;">;</span>
<span style="color:#b4b4b4;">}</span>
</div></code></pre>

---

<!--header: 'Complement & underlying type'-->

<pre><code><div style="font-size:90%"><span style="color:#569cd6;">enum</span>&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">flag_0</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x1</span><span style="color:#b4b4b4;">,</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#b8d7a3;">flag_1</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b5cea8;">0x2</span>
<span style="color:#b4b4b4;">};</span>
<span style="color:#569cd6;">inline</span>&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#b4b4b4;">operator</span><span style="color:#b4b4b4;">~</span>&nbsp;<span style="color:#b4b4b4;">(</span><span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9a9a9a;">x</span><span style="color:#b4b4b4;">)</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#d8a0df;">return</span>&nbsp;<span style="color:#569cd6;">static_cast</span><span style="color:#b4b4b4;">&lt;</span><span style="color:#4ec9b0;">flags</span><span style="color:#b4b4b4;">&gt;(~</span><span style="color:#569cd6;">static_cast</span><span style="color:#b4b4b4;">&lt;</span><span style="color:#569cd6;">int</span><span style="color:#b4b4b4;">&gt;(</span><span style="color:#9a9a9a;">x</span><span style="color:#b4b4b4;">));</span>
<span style="color:#b4b4b4;">}</span>
<span style="color:#57a64a;">//&nbsp;other&nbsp;operators&nbsp;...</span>
<span style="color:#569cd6;">void</span>&nbsp;<span style="color:#dcdcaa;">foo</span><span style="color:#b4b4b4;">()</span>&nbsp;<span style="color:#b4b4b4;">{</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b8d7a3;">flag_0</span>&nbsp;<span style="color:#b4b4b4;">|</span>&nbsp;<span style="color:#b8d7a3;">flag_1</span><span style="color:#b4b4b4;">;</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#4ec9b0;">flags</span>&nbsp;<span style="color:#9cdcfe;">mask</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#b4b4b4;">~</span><span style="color:#b8d7a3;">flag_1</span><span style="color:#b4b4b4;">;</span>
&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">=</span>&nbsp;<span style="color:#9cdcfe;">f</span>&nbsp;<span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#9cdcfe;">mask</span><span style="color:#b4b4b4;">;</span>
<span style="color:#b4b4b4;">}</span>
</div></code></pre>

* unscoped enum w/o explicit underlying type: two types inferred:
  * `underlying_type`
  * hypothetical integer value type with minimal width such that all enumerators can be represented -> defining the valid value-range
<!-- * explicit underlying type / `class` required to prevent UB -->
* `complement<flags>` uses `underlying_type_t<flags>` for value

---
<!--header: 'Boost.Flags'-->

# Design Rationale of Boost.Flags

* type-safety
  * enforce compatible types for binary operators
  * `complement` for `operator~`

* non-intrusive

* zero-overhead in optimized builds

* everything is `constexpr`

* operators are found by ADL

* at least C++11, use newer features if available

<!-- ---
<!--header: 'Boost.Flags'

# Boost.Flags: life example...

comparing with `BOOST_BITMASK` -->

---
<!--header: 'Boost.Flags'-->

# Boost.Flags: life example...

https://godbolt.org/z/5ffq6eGaW



---
<!--header: 'Orders'-->

# Orders on flags-enumerations

* induced by `<` on underlying integer type
  * total order
  * depends on enumerator-values
  * Syntax: used for sorting, sorted containers
* induced by entailment of flags
  * `a < b`  <=>  "flags in a" $\subset$ "flags in b"
  * partial order
  * independent of enumerator-values
  * Semantics

---

* sorted containers / algorithms require total order
  * `BOOST_FLAGS_SPECIALIZE_STD_LESS(E)` for sorted containers / std::algorithms
  * NOT working with `std::ranges`: calling `operator <`

* enums abstract away underlying values
  * program logic should not depend on underlying values

* `BOOST_FLAGS_DELETE_REL(E)` deletes all relational operators

* constexpr objects `total_order`, `partial_order`

* functions for partial-order comparison: `subset`, `subseteq`, `intersect`, `disjoint`

---

# Boost.Flags: additional features

* `make_null`, `make_if`
* `modify`, `modify_inplace`
* `add_if`, `add_if_inplace`
* `remove_if`, `remove_if_inplace`
* `get_underlying`, `from_underlying`

* Flags-generator: 
  * `flags_from_to(first, last)` iterates over all flags from `[first, ..., last]` 
  * `flags_to(last)` iterates over all flags from `[1, ..., last]` 



---
<!--header: 'Boost.Flags'-->

# Boost.Flags: applying to existing code-bases

* change macro / integer constants to enums
* opt-in by with `BOOST_FLAGS_ENABLE(E)`
* use scoped enums, `using enum E;`
* change interfaces from integer-types to enums
* minor adjustments:
  * <pre><code><div style="font-size:100%"><span style="color:#d8a0df;">if</span>&nbsp;<span style="color:#b4b4b4;">(</span><span style="color:#9cdcfe;">flags</span>&nbsp;<span style="color:#b4b4b4;">&amp;</span>&nbsp;<span style="color:#b8d7a3;">LBS_NOTIFY</span><span style="color:#b4b4b4;">)</span>&nbsp;<span style="color:#b4b4b4;">{...}</span>&nbsp;&nbsp;&nbsp;<span style="color:#57a64a;">//&nbsp;scoped&nbsp;enum&nbsp;-&gt;&nbsp;bool</span></div></code></pre>
  * <pre><code><div style="font-size:100%"><span style="color:#b4b4b4;">(</span>cond&nbsp;<span style="color:#b4b4b4;">?</span>&nbsp;<span style="color:#b8d7a3;">LBS_NOTIFY</span>&nbsp;<span style="color:#b4b4b4;">:</span>&nbsp;<span style="color:#b5cea8;">0</span><span style="color:#b4b4b4;">)</span></div></code></pre>
    <pre><code><div style="font-size:100%"><span style="color:#c8c8c8;">boost</span><span style="color:#b4b4b4;">::</span><span style="color:#c8c8c8;">flags</span><span style="color:#b4b4b4;">::</span><span style="color:#dcdcaa;">make_if</span><span style="color:#b4b4b4;">(</span><span style="color:#b8d7a3;">LBS_NOTIFY</span><span style="color:#b4b4b4;">,</span>&nbsp;cond<span style="color:#b4b4b4;">)</span></div></code></pre>
* also works with enums in classes: `BOOST_FLAGS_ENABLE_LOCAL(E)`


---

# Boost.Flags: in real code

* EBSILON&reg;Professional: 122 uses of `BOOST_FLAGS_ENABLE...`

* Clang source code: enabled 16 enums - no errors detected

* Github-repo: tests run on a large matrix:
  * linux, windows, mac
  * clang (3.9 - 17), gcc (4.8-13), msvc (v140 - v143)
  * C++11 - C++23

---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| zero-overhead    | -      | +          |  +             | +    |
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| zero-overhead    | -      | +          |  +             | +    |
| `constexpr`      | -      | +/-        |  +/-           | N/A  |
| &nbsp; | 
| &nbsp; | 
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| zero-overhead    | -      | +          |  +             | +    |
| `constexpr`      | -      | +/-        |  +/-           | N/A  |
| type-safe bin-ops| -      | +          |  -             | +    |
| &nbsp; | 
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| zero-overhead    | -      | +          |  +             | +    |
| `constexpr`      | -      | +/-        |  +/-           | N/A  |
| type-safe bin-ops| -      | +          |  -             | +    |
| large underlying_t| -     | +          |  -             | +    |
| &nbsp; | 


---

# Some other available Flags libraries

|<span style="visibility:hidden"> type-safe bin-ops</span>| QFlags | foonathan  | BOOST_BITMASK  | .net |
|------------------|:------:|:----------:|:--------------:|:----:|
| non-intrusive    | -      | -          |  +             | +    |
| zero-overhead    | -      | +          |  +             | +    |
| `constexpr`      | -      | +/-        |  +/-           | N/A  |
| type-safe bin-ops| -      | +          |  -             | +    |
| large underlying_t| -     | +          |  -             | +    |
| `complement`     | -      | +          |  -             | -    |


<!-- ---

<!--header: 'the hop-experts'-- >

<center>

# this poll is presented to you by the hop-experts

</center>

<style>
 .hop_experts { 
     border: none;
  margin: none;
  border-collapse: collapse;
  width:100%;
  background-color: rgb(34,34,34);

      }


.hop_experts_td {
    box-sizing: content-box;
  border: none;
  margin: none;
  background-color: rgb(34,34,34);
}
</style>

<div style="margin: auto;">
<table class="hop_experts">
<tr style="border: none">
<td class="hop_experts_td" style="text-align:right; ">

# Luna

</td>

<td class="hop_experts_td" style="width:580px">

![width:500px center](pictures/hop_experts_clipped.jpg)

</td>

<td class="hop_experts_td" >

# Rolf

</td>

</tr>
</table>
</div>

 -->



---

<!--header: 'hopping on'-->

# Code & Documentation:

<h1> <a href="https://github.com/tobias-loew/flags">github.com/tobias-loew/flags</a> </h1>
  



---

<!-- 
    .vsselected{background-color:#264f78;border-style: solid;border-width: 1px;border-color:#889bad}
 -->


<style>
img[alt~="center"] {
  display: block;
  margin: 0 auto;
}
</style>


![width:700px center](pictures/Luna_Meersau.png)

<center>

# questions / remarks?

</center>






# FILES
## MKTAG
1. ### The legacy MKTAG

> is saving every single combination of triangles needed to draw a shape.

Number of bytes: 
```
n>2
(n-2)*72
```
where ```n``` is the number of verteces

2. ### The index MKTAG
> is saving all the vertece data, number of verteces and the their index order

```
(x usually is equal to n-2)
8+24n+12x
```
where ```n``` is the number of verteces

and ```x``` is the number of faces

## comparison
**The index MKTAG** is always better than **The legacy MKTAG**

because when the number of verteces is greater than **1** and ```x = n-2```, index MKTAG performs better, but legacy MKTAG, can only save Array graphics with n > 2.

(
8+24n+12(n-2) < (n-2)*72
)
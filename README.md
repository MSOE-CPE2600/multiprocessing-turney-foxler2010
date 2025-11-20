# Systems Programming Lab 11 Multiprocessing

## mandelmovie

My implementation of the `mandelmovie` program executes the original mandel program directly.
It starts off by parsing program arguments in a similar way to `mandel`, with the added option
of `-c` to specify the number of child processes to spawn (default 10).

Since multiple frames are generated with each being slightly different, that means we need to
have a list of scale parameters with each value being a little different than the next to achieve the zooming out effect. In the case of the Mandelbrot set, in order for the zoom to look good,
an exponential growth formula is best suited for the job. My original goal was to have the
animation pan as well as zoom so that it would end on a centered image of the Mandelbrot set,
but I was unable to find a formula for the x and y values that did not move the camera to a
"boring" part of the set away from the area of focus. Instead, I have chosen to leave these
values constant and end the zoom slightly off center.

Another possible improvement would be to add an ease-in and ease-out to the exponential growth
functions that calculates the scale values. This would give the animation a smooth start and stop
instead of the quick and jerky beginning and end seen in my video.

## How # of processes affects runtime

As for the runtime, please see the image below for my graph and the data I measured to compare
how the program ran with different numbers of child processes:

![mandelmovie Results Chart](results/results.png)

As shown, the "Real" time (which is what we actually experience)
decreases as the number of processes goes up. My computer only has
12 cores, though, so at 20 processes it must have been doing some "multitasking"
in order for things to work. This can actually slow down a program,
or at the very least cause it to not realize any performance gain.

I have had much success using my program's default of 10 children,
and it is of course less than the number of cores I have, so
that is what I'll be recommending as the best amount of multiprocessing
for this scenario.

### Cool image to zoom out from:
`./mandel -s .0001 -x -1.249705 -y .111053 -o kool.jpg`

`./mandel -W 8000 -H 8000 -s .0001 -x -1.249705 -y .111053 -o kool_big.jpg`

Video of `kool_big` located at `kool_big/kool_big.mpg`. Need to use VLC to play it.
<video src="kool_big/kool_big.mpg" width="1280" type="video/mpeg" controls></video>

### Output from when I generated kool_big

![kool_big terminal output 1](kool_big/1.png)

![kool_big terminal output 2](kool_big/2.png)

It took almost an hour to generate all those huge frames but it was worth it.

FFmpeg crashed my whole system the first couple times I tried to encode it.

I guess 8000x8000 pixel images are hard to encode.... who knew!

Solution was to use `-scale "1280:-1"` to make the video smaller.

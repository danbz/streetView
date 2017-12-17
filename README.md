# based on ofxStreetView

forked from http://patriciogonzalezvivo.com 

[ ![stokes croft](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-19.45.12-e1513543600881.png) ](http://buzzo.com/streetview/)

[ ![stokes croft](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.54.26-e1513543978734.png) ](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.54.26-300x261.png/)

[ ![backfields lane](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.53.21-e1513544088818.png) ](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.53.21-300x210.png)



## How it works

By doing the call:

	http://cbk0.google.com/cbk?output=xml&panoid=[pano_id]&dm=1

We get information that looks like [this](http://maps.google.com/cbk?output=xml&cb_client=maps_sv&v=4&dm=1&hl=en&panoid=ki_KzVWkE87EgkPWg3QPXg) 

At <deptMap> you can seee a depth image encoded in base64 (and zlib compressed)

This addon will construct the panoramic image (that you can get with ```getTextureReference()``` ) and then construct a 3D Mesh using the DepthMap information.

## Credits
- [ StreetView Explorer of Paul Wagener](https://github.com/PaulWagener/Streetview-Explorer)

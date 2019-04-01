# NGT-php

NGT-php is a PHP bindings for NGT.

[NGT](https://github.com/yahoojapan/NGT) Neighborhood Graph and Tree for Indexing High-dimensional Data

## Requirements

libngt

```
$ curl -fSL "https://github.com/yahoojapan/NGT/archive/v1.6.2.tar.gz" -o "ngt-1.6.2.tgz"
$ tar xf ngt-1.6.2.tgz
$ cd NGT-1.6.2
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make -j $(nproc)
$ sudo make install
```

## Building NGT for PHP

```
$ cd ngt-php
$ phpize
$ ./configure
$ make -j $(nproc)
$ sudo make install
```

edit your php.ini and add:

```
extension=ngt.so
```

## Class synopsis

```php
NGT {
    public __construct ( void )
    public void open ( string filename[, int readOnly ...] )
    public void save ( void )
    public void close ( void )
    public void insert ( array data[, int objectCount=1, int numThreads=8] )
    public void insertList ( array data[][, int numThreads=8] )
    public array search ( array query[, int row=10, float epsilon = 0.1, int edgeSize = -1] )
    public void remove ( int id )
    public array getObject ( int id )
    public static CreateDB ( string filename, int dimension[, ...])
}
```

## Table of Contents

* [NGT::__construct](#__construct)
* [NGT::open](#open)
* [NGT::save](#save)
* [NGT::close](#close)
* [NGT::insert](#insert)
* [NGT::insertList](#insertlist)
* [NGT::search](#search)
* [NGT::remove](#remove)
* [NGT::getObject](#getobject)
* [NGT::CreateDB](#createdb)

-----

### <a name="__construct">NGT::__construct()

Instantiates a NGT object.

```php
$index = new Croco\NGT\Index();
```

-----

### <a name="open">void NGT::open(string filename)

open a database.


### <a name="close">void NGT::close()

close a database.

```php
$index = new Croco\NGT\Index();

$database = 'index';
$index->open($database);
$index->close();
```

-----

### <a name="insert">void NGT::insert(array data)

insert a object.


### <a name="save">void NGT::save()

save a database.

```php
$index = new Croco\NGT\Index();
$index->open('index');

$index->insert([0.033712, -0.058824, 0.08323, 0.072274, ... ...]);

$index->save();
$index->close();
```

-----

### <a name="insertlist">void NGT::insertList()

insert a object.

```php
$index = new Croco\NGT\Index();
$index->open('index');

$index->insertList([
    [0.033712, -0.058824, 0.08323, 0.072274, ... ...],
    [075103, 0.059359, 0.083976, 0.04961, ... ...],
    [ -0.026512, 0.048607, -0.021153, 0.043541, ... ...]
]);

$index->save();
$index->close();
```

-----

### <a name="search">array NGT::search()

search the nearest neighbors.

```php
$index = new Croco\NGT\Index();
$index->open('index');

$result = $index->search([0.074936, 0.05886, 0.083432, 0.049463, ... ...]);

$index->close();

print_r($result);
```

```php
[
    [
        'Rank'     => 1, 
        'ID'       => 3
        'Distance' => 0.00971914
    ],
    [
        'Rank'     => 2, 
        'ID'       => 52
        'Distance' => 0.04755864
    ],
            :
            :
            :
]
```

-----

### <a name="remove">void NGT::remove()

```php
$index = new Croco\NGT\Index();
$index->open('index');

$index->remove(4);

$index->save();
$index->close();
```

-----

### <a name="getobject">array NGT::getObject()

```php
$index = new Croco\NGT\Index();
$index->open('index');

echo $index->getObject(3);

$index->close();
```

```
[0.074936, 0.05886, 0.083432, 0.049463, ... ...]
```
-----

### <a name="createdb">void NGT::CreateDB()

```php
Croco\NGT\Index::CreateDB(
    'index',    // database path
    300,        // dimension
    10,         // edge size for creation
    40,         // edge size for search
    'L2',       // distance type
    'Float'     // object type [Integer, Float]
);


$index = new Croco\NGT\Index();
$index->open('index');
$index->close();
```

#### DistanceType
| 距離関数          | 内容 |
|:-----------------|:----------|
| None             | なし |
| L1               | L1距離 |
| L2               | L2距離（デフォルト） |
| Hamming          | ハミング距離 |
| Angle            | 角度 |
| Cosine           | コサイン類似度 |
| NormalizedAngle  | 角度：正規化して保存 |
| NormalizedCosine | コサイン類似度：正規化して保存 |

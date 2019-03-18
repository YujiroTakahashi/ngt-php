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
    public void insert ( array data[, int objectCount=1, int numThreads=8] )
    public array search ( array query[, int row=10, float epsilon = 0.1, int edgeSize = -1] )
    public void remove ( int id )
    public array getObject ( int id )
    public static createDB ( string filename, int dimension[, ...])
}
```

## Table of Contents

* [NGT::__construct](#__construct)
* [NGT::open](#open)
* [NGT::insert](#insert)
* [NGT::search](#search)
* [NGT::remove](#remove)
* [NGT::getObject](#getobject)
* [NGT::createDB](#createdb)

-----

### <a name="__construct">NGT::__construct()

Instantiates a NGT object.

```php
$index = new NGT();
```

-----

### <a name="open">void NGT::open(string filename)

open a database.

```php

$database = 'index';
$index->opan($database);
```

-----

### <a name="insert">void NGT::insert()

insert a object.

```php
$index->insert([0.033712, -0.058824, 0.08323, 0.072274, ... ...]);
```

-----

### <a name="search">array NGT::search()

search the nearest neighbors.

```php
$result = $index->search([0.074936, 0.05886, 0.083432, 0.049463, ... ...]);
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
$index->remove(4);
```

-----

### <a name="getobject">int NGT::getObject()

```php
echo $index->getObject(3);
```

```
[0.074936, 0.05886, 0.083432, 0.049463, ... ...]
```
-----

### <a name="createdb">int NGT::createDB()

```php
Index::createDB(
    'index',
    300
);
```

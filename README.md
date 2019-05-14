# NGT-php

NGT-php is a PHP bindings for NGT.

[NGT](https://github.com/yahoojapan/NGT) Neighborhood Graph and Tree for Indexing High-dimensional Data

## Requirements

libngt

```
$ curl -fSL "https://github.com/yahoojapan/NGT/archive/v1.7.3.tar.gz" -o "ngt-1.7.3.tgz"
$ tar xf ngt-1.7.3.tgz
$ cd NGT-1.7.3
$ mkdir build
$ cd build
$ cmake .. -DNGT_AVX_DISABLED=ON -DCMAKE_BUILD_TYPE=Release
$ make -j $(nproc)
$ sudo make install
```

[libprotobuf](https://github.com/protocolbuffers/protobuf/)

```
$ curl -fSL "https://github.com/protocolbuffers/protobuf/releases/download/v3.7.1/protobuf-cpp-3.7.1.tar.gz" -o "protobuf-3.7.1.tgz"
$ tar xf protobuf-3.7.1.tgz
$ cd protobuf-3.7.1
$ ./configure
$ make -j $(nproc)
$ make check
$ sudo make install
```


[libcroco_ngt](https://github.com/YujiroTakahashi/ngt-c/blob/master/README.md)（ngt-c）

```
$ curl -fSL "https://github.com/YujiroTakahashi/ngt-c/archive/v0.1.0.tar.gz" -o "croco-ngt-0.1.0.tgz"
$ tar xf croco-ngt-0.1.0.tgz
$ cd ngt-c-0.1.0
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
Croco::NGT {
    public __construct (void)
    public void create (int dimension [, int edgeSizeForCreation, int edgeSizeForSearch, string distanceType, string objectType])
    public string getObjectString ( void )
    public string getDistanceString ( void )
    public string getLeafNodeString ( void )
    public string getInternalNodeString ( void )
    public void setObjectString ( string data )
    public void setDistanceString ( string data )
    public void setLeafNodeString ( string data )
    public void setInternalNodeString ( string data )
    public void importIndex ( void )
    public void exportIndex ( void )
    public void insert ( array data[, int objectCount=1, int numThreads=8] )
    public void insertList ( array data[][, int numThreads=8] )
    public array search ( array query[, int row=10, float epsilon = 0.1, int edgeSize = -1] )
    public void remove ( int id )
    public array getObject ( int id )
}
```

## Table of Contents

* [Croco::NGT::__construct](#__construct)
* [Croco::NGT::create](#create)
* [Croco::NGT::insert](#insert)
* [Croco::NGT::insertList](#insertlist)
* [Croco::NGT::search](#search)
* [Croco::NGT::remove](#remove)
* [Croco::NGT::getObject](#getobject)
* [Croco::NGT::exportIndex](#export)
* [Croco::NGT::getObjectString](#export)
* [Croco::NGT::getDistanceString](#export)
* [Croco::NGT::getLeafNodeString](#export)
* [Croco::NGT::getInternalNodeString](#export)
* [Croco::NGT::setObjectString](#import)
* [Croco::NGT::setDistanceString](#import)
* [Croco::NGT::setLeafNodeString](#import)
* [Croco::NGT::setInternalNodeString](#import)
* [Croco::NGT::importIndex](#import)


-----

### <a name="__construct">NGT::__construct()

Instantiates a NGT object.

```php
$index = new Croco\NGT\Index();
```

-----

### <a name="open">void NGT::create(int dimension [, int edgeSizeForCreation, int edgeSizeForSearch, string distanceType, string objectType])

create a index.

```php
$index = new Croco\NGT\Index();

$dimension = 300;
$edgeSizeForCreation = 10;
$edgeSizeForSearch = 40;
$distanceType = 'L2';
$objectType = 'Float';

$index->create(
    $dimension,
    $edgeSizeForCreation,
    $edgeSizeForSearch,
    $distanceType,
    $objectType
);
```

-----

### <a name="insert">void NGT::insert(array data)

insert a object.


### <a name="insertlist">void NGT::insertList()

insert a object.

```php
$index = new Croco\NGT\Index();
$index->create(300);

$index->insertList([
    [0.033712, -0.058824, 0.08323, 0.072274, ... ...],
    [075103, 0.059359, 0.083976, 0.04961, ... ...],
    [ -0.026512, 0.048607, -0.021153, 0.043541, ... ...]
]);
```

-----

### <a name="search">array NGT::search()

search the nearest neighbors.

```php
$index = new Croco\NGT\Index();
$index->create(300);

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
$index = new Croco\NGT\Index();
$index->open('index');

$index->remove(4);
```

-----

### <a name="getobject">array NGT::getObject()

```php
$index = new Croco\NGT\Index();
$index->create(300);

echo $index->getObject(3);
```

```
[0.074936, 0.05886, 0.083432, 0.049463, ... ...]
```
-----


### <a name="export">void NGT::exportIndex()

```php
$index = new Croco\NGT\Index();
$index->create(300);

$index->insertList([
    [0.033712, -0.058824, 0.08323, 0.072274, ... ...],
    [075103, 0.059359, 0.083976, 0.04961, ... ...],
    [ -0.026512, 0.048607, -0.021153, 0.043541, ... ...]
]);


$db = new \PDO(......);
$stmt = $db->prepare(
    'INSERT INTO `index` (`objects`, `distances`, `leafNodes`, `internalNodes`) '.
                 'VALUES (:objects,  :distances,  :leafNodes,  :internalNodes)'
);


$index->exportIndex();

$stmt->bindValue(':objects',       $index->getObjectString(),       \PDO::PARAM_LOB);
$stmt->bindValue(':distances',     $index->getDistanceString(),     \PDO::PARAM_LOB);
$stmt->bindValue(':leafNodes',     $index->getLeafNodeString(),     \PDO::PARAM_LOB);
$stmt->bindValue(':internalNodes', $index->getInternalNodeString(), \PDO::PARAM_LOB);
$stmt->execute();
```

-----


### <a name="import">void NGT::importIndex()

```php
$index = new Croco\NGT\Index();
$index->create(300);

$db = new \PDO(......);

$stmt = $db->prepare(
    'SELECT `objects`, `distances`, `leafNodes`, `internalNodes` FROM `indexblob` WHERE `id` = :id'
);
$stmt->bindValue(':id', 1, \PDO::PARAM_INT);
$stmt->execute();

$row = $stmt->fetch(\PDO::FETCH_ASSOC);
$stmt->closeCursor();

$index->setObjectString($row['objects']);
$index->setDistanceString($row['distances']);
$index->setLeafNodeString($row['leafNodes']);
$index->setInternalNodeString($row['internalNodes']);

$index->importIndex();

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


#### Create Table
```sql
CREATE TABLE `index` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `objects` mediumblob NOT NULL,
  `distances` mediumblob NOT NULL,
  `leafNodes` mediumblob NOT NULL,
  `internalNodes` mediumblob NOT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=ascii COLLATE=ascii_bin
```

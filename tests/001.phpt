--TEST--
Check for ngt presence
--SKIPIF--
<?php if (!extension_loaded("ngt")) print "skip"; ?>
--FILE--
<?php 
echo "ngt extension is available";
?>
--EXPECT--
ngt extension is available
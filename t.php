<?php
greeting();
echo pltest_add(111, 222),PHP_EOL;
echo greeting_word('greeting ,', 'zhangsan'), PHP_EOL;

$str = 'pltest_md5salt';
$salt = 'hello';
echo "------------\nPHP: md5($str.$salt)", PHP_EOL;
echo md5($str.$salt), PHP_EOL;
echo "extention:\n";
pltest_md5salt($salt);

echo "\n------------\n\n";

//copy from onesdk {{{
function toEnSign($data, $key,$osdkId=0,$app="sign",$category="log")
{
    ksort($data);
    $str = array();
    foreach ($data as $k => $v) {
        $str[] = $k . "=" . urldecode($v);
    }
    $strData = implode("&", $str) . $key;
    $sign = md5($strData);

    return $sign;
}
function toDeSign($data, $sign, $key, $app = "")
{
    ksort($data);
    $str = array();
    foreach ($data as $k => $v) {
        $str[] = $k . "=" . urldecode($v);
    }
    $strData = implode("&", $str) . $key;
    $waitSign = md5($strData);
    return $waitSign == $sign;
}
// }}}


$data = ['a'=>'b', 'c'=>'d'];
$key = 'kkkeeeyyy';

$enstr = toEnSign($data, $key);

echo "enstr: ", $enstr;
echo "destr: ", toDesign($data, $enstr, $key), PHP_EOL;







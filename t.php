<?php
greeting();

d(PHP_EOL);

$re = pltest_add(1, 2);
d($re);

d(greeting_word('greeting ,', 'zhangsan'));

$str = 'pltest_md5salt';
$salt = 'hello';
d("------------\nPHP: md5($str.$salt)");
d(md5($str.$salt));
d("extention:");
pltest_md5salt($salt);

d(PHP_EOL);

/*
 * display string
 */
function d($str){
    echo $str;
    echo PHP_EOL;
}
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







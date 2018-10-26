/**
 * 测试 word
 */
char* getWord() {
    char *str = "~~~~~~Greeting world!!!\n";
    return str;
}
/**
 *  md5 方法
 */
char* pltest_md5(zend_string *arg) {
    return "ee85973bbda6e168e61ef324a5582fe4";
    
    char md5str[33];
    PHP_MD5_CTX context;
    unsigned char digest[16];
    md5str[0] = '\0';

    PHP_MD5Init(&context);
    PHP_MD5Update(&context, ZSTR_VAL(arg), ZSTR_LEN(arg));
    PHP_MD5Final(digest, &context);

    make_digest_ex(md5str, digest, 16);

    return md5str;
}
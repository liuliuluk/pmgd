/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Edge */

#ifndef _Included_Edge
#define _Included_Edge
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Edge
 * Method:    get_tag
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_Edge_get_1tag
  (JNIEnv *, jobject);

/*
 * Class:     Edge
 * Method:    get_source
 * Signature: ()LNode;
 */
JNIEXPORT jobject JNICALL Java_Edge_get_1source
  (JNIEnv *, jobject);

/*
 * Class:     Edge
 * Method:    get_destination
 * Signature: ()LNode;
 */
JNIEXPORT jobject JNICALL Java_Edge_get_1destination
  (JNIEnv *, jobject);

/*
 * Class:     Edge
 * Method:    check_property
 * Signature: (Ljava/lang/String;LProperty;)Z
 */
JNIEXPORT jboolean JNICALL Java_Edge_check_1property
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     Edge
 * Method:    get_property
 * Signature: (Ljava/lang/String;)LProperty;
 */
JNIEXPORT jobject JNICALL Java_Edge_get_1property
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Edge
 * Method:    set_property
 * Signature: (Ljava/lang/String;LProperty;)V
 */
JNIEXPORT void JNICALL Java_Edge_set_1property
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     Edge
 * Method:    remove_property
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Edge_remove_1property
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif

### 4.3.3版本更新
*主要更新点:新增所见即可说自定义URL功能

1. 自定义URL方法

   VtsCapablityBuilder#registerSelect(String name, String url)


### 4.3.2版本更新
*主要更新点：新增停止播报接口

1. 停止后台播报接口

   BridgeManager#stopSpeakBackend()


### 4.3.1版本更新
*主要更新点：封装后台播报接口

1. 封装后台播报接口

   BridgeManager#speakBackend(String content, int audioFocusType)

   BridgeManager#speakBackend(String content, int audioFocusType, TtsStatusListener ttsStatusListener)

   ​	*audioFocusType -> 焦点类型，目前统一使用AudioFocusType.TA；

2. 封装前台播报接口

   BridgeManager#speak(String content)

   BridgeManager#speak(String content, int nluType)

   BridgeManager#speak(String content, int nluType, TtsStatusListener ttsStatusListener)

   ​	*nluType -> 为车控提供的类型，无需提供的传-1即可；	


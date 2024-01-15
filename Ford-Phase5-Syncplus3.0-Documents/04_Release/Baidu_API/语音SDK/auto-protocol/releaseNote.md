### 1.0更新点
* 主要更新点：因场景引擎需为VPA增加接口
1. 在之前唤醒接口上新增一个参数：
/**
唤醒 
@param wpMode 唤醒模式
0： 语音唤醒
1： 场景引擎唤醒
*/
wakeup (int wpMode);
 
调用方法如下：
IpcBridge.getInstance().get(VoiceAssistantProxy.class).wakeup(int wpMode);

### 1.1 更新点
* 主要更新点：场景推荐VpaAction
/**
* 场景推荐
*/
String SCENE_RECOMMEND = "TYPE_SCENE_RECOMMEND";

引用方法：
Constants.VpaActionType.SCENE_RECOMMEND

### 1.2 更新点
* 主要更新点1：场景推荐正负反馈接口
/**
* 场景引擎下语义正负反馈
*
* @param positive 正反馈
*/
void onFeedbackInScene(boolean positive);
--方法所属类：ConversationListener

* 主要更新点2：onWakeUp()新增type参数
/**
* 唤醒
*
* @param index 唤醒的音区 0 主驾  1 副驾
* @param type  唤醒方式
*/
void onWakeUp(int index, int type);
--方法所属类：ConversationListener


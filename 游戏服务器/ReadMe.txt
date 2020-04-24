
修改流程:

1. 修改Attribute属性
	1) 修改CMD_SubGame.h 中的KindID          -- 向项目经理索要
	2）修改CMD_SubGame.h 中的NodeID          -- 向项目经理索要
	3) 修改CMD_SubGame.h 中的GAME_GENRE		TODO 子游戏删除，放在数据库KindItem表中

1. 特殊牌型 
	在 消息定义/cardType.h中设置特殊牌型对应的value -- 这与xml中的CAME_SCORE_MODE_CLASSIC 和  CAME_SCORE_MODE_CRAZY中的应该一致

4.设置子游戏特殊函数
	1)在SubGameRule.h 中增加 AppointBanker()
	2)在SubGameRule.h 中增加 AnsysCardsType()
	3)在SubGameRule.h 中增加 Settle()
	4)在GameData.cpp  中处理 Operate()

5. 修改xml配置文件
	1) 按牌型数据设置xml
	2) xml名字 应该为 "KindID_NODEID.xml"

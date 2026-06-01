# 徐剑锋 / XU JIANFENG

::: left

icon:info 男/1997.9

icon:phone +86-13409790757

:::

::: right

**求职意向：** AI Agent/AI 应用工程师 （深圳)

icon:email <xujf_work@foxmail.com>
:::

***

## 教育背景

:::left
**新加坡国立大学（NUS）- 人工智能 & 创新 理学硕士**
:::
:::right
新加坡 | **2025.08 - 2026.6**
:::

:::left
**华中科技大学 - 物理学 理学学士**
:::
:::right
武汉 | **2015.09 - 2019.06**
:::

***

## 核心优势

- **Agent 全链路开发能力**（独立开发：[BI-Agent](https://bi-agent.tech/)）：从 0 到 1 独立完成 Agent 系统的架构设计、开发与上线，两套系统已落地（字节跳动已上线 + Curveseries 交付客户）；具备从业务需求出发独立设计 Agent 架构的能力，不依赖框架封装。

- **LLM 工程优化能力**：向头部开源 Agent 项目Hermes Agent贡献的 [KV cache 优化方案](https://github.com/NousResearch/hermes-agent/pull/25434) 已合并入主干，定位并修复其缓存命中根因问题，单次复盘调用 token 成本下降 **89%**、端到端 token 成本下降 **26%**；具备从 LLM 底层机制反推根因、推动工程落地的能力。

***

## 工作经历

:::left
**字节跳动 | 测试开发/Agent 开发 | 互娱研发-基础体验 | 深圳**
:::
:::right
**2021.03 - 2025.07**
:::

- **成本分析归因AI Agent**：2024年底主动引入LLM技术，推动分析能力系统性升级：纯代码自动化报告 → 固定Workflow报告 → React + NL2SQL 架构的智能归因Agent（MCP协议集成HiveSQL查询，SQL准确率90%+；归因效率提升 **30倍**（M+5天 → T+1天）；问题召回率 **95%+**，识别成本劣化 **1.7%**，年度节省**数千万元**；
- **体验评测 × 数据挖掘**：负责抖音直播视频画质、音质、成本等多维体验指标的全链路评测，创新性融合线上数据挖掘（因果推断、相关性分析）与线下主客观评测，构建"数据驱动的高价值问题发现体系"；系统性识别体验瓶颈并推动产研落地解决，带动用户留存时长（LT）提升 **0.6%**
- **成本竞品评测**：主导构建抖音直播成本评测完整方案——系统衡量单位成本合理性、与竞品进行基准对比、识别可优化方向并推进落地；作为项目核心，推动**成本下降 2.3%**

:::left
**OPPO | 相机画质评测工程师 | 产品研发部 | 深圳**
:::
:::right
**2019.07 - 2021.03**
:::

- 负责旗舰机型相机画质全面评测（MTF / Delta-E / SNR / 动态范围），制定客观评测标准与竞品基准体系；与算法团队协作推进AI成像迭代（去噪 / 超分 / HDR融合），采集多场景训练数据并验证算法上线收益

***

## 项目经历

:::left

### Curveseries AI 大宗商品智能分析系统 | NUS 企业合作项目


:::
:::right
**2025.11 - 2026.5**
:::

:::left
**角色：** P0模块负责人（系统架构 / Agent实现 / Prompt工程 / 工具设计）
:::
:::right
:::

- 项目背景：合作方 Curveseries（服务壳牌、高盛等100+机构客户），构建大宗商品 AI Agent——自动生成可直接用于交易决策的市场分析报告
- Agent 架构：主导设计双阶段 Agentic Loop——Research Phase（实时行情 + 分时段新闻召回 + 7品种供需框架按需注入）→ Format Enforcement Phase（数值校验 + 引用溯源 + 结构化输出）；针对 LLM 幻觉，强制全链路 citation grounding，达到 100% citation rate
- 跨会话记忆：设计三级持久化记忆架构（Daily → Weekly → Monthly 自动归并），Tiered Recall 按时间衰减注入 context window，Agent 自主判断 SKIP / NARROW / FULL 检索策略，每次生成节省 2-4 次 API调用
- 工具链与成本：自研 3 个业务工具（行情 / 新闻 /知识库懒加载）；新闻按时段分段召回确保分钟级突发事件零遗漏；Memory-aware 检索策略减少冗余调用，API成本降低约 80% 
- 交付验收：新闻实时变动无法构建静态 benchmark，采用 Live Evaluation——日报交付交易员按维度打分；Golden Case 前置对齐 + 三维度 UAT，P0 模块通过创始人多轮评审，显著优于同类产品

***

## 技能清单

- **AI/Agent**：Agent系统（ReAct / Multi Agent）、RAG、Prompt Engineering、MCP、Anthropic SDK
- **工程与数据**：Python（精通）、HiveSQL（熟练）、Spark、FastAPI、Git | 英语雅思 6.5



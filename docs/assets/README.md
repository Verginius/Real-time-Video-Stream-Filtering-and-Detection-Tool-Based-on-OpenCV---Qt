# 文档资源索引

本目录存放所有模块设计文档对应的可视化图表（Mermaid 源码）。

## 文件列表

| 文件 | 对应设计文档 | 包含图表 |
|------|------------|---------|
| [arch-overview.md](arch-overview.md) | 全局 | 模块分层架构图、数据流向图、组件依赖图 |
| [class-VideoSource.md](class-VideoSource.md) | DD-VIN | 类继承图、摄像头枚举时序、文件播放控制时序、屏幕区域选择流程 |
| [class-Filter.md](class-Filter.md) | DD-FLT | 类继承图、滤镜链处理流程、各滤镜算法流程、参数热更新时序 |
| [class-Detection.md](class-Detection.md) | DD-DET | 类关系图、YOLOv8 推理流水线、跳帧检测时序、坐标映射、GPU 切换流程 |
| [seq-FrameLoop.md](seq-FrameLoop.md) | DD-CTL | 线程架构图、完整帧循环时序、启动关闭时序、跨线程安全矩阵、帧数据生命周期 |
| [gui-widget-tree.md](gui-widget-tree.md) | DD-GUI | 控件树结构、信号槽连接图、滑块联动状态机、帧渲染流程、主题切换 |
| [flow-Export.md](flow-Export.md) | DD-EXP | 类关系图、录制状态机、录制时序、截图流程、CSV/JSON 导出流程、输出目录规划 |

## 使用方式

所有图表使用 [Mermaid](https://mermaid.js.org/) 语法编写，可通过以下方式渲染：

- **VS Code**：安装 [Mermaid Preview](https://marketplace.visualstudio.com/items?itemName=bierner.markdown-mermaid) 插件
- **GitHub / GitLab**：直接在 Markdown 中渲染（原生支持）
- **在线工具**：[mermaid.live](https://mermaid.live)

## 图表类型说明

| Mermaid 类型 | 用途 |
|------------|------|
| `classDiagram` | 类继承与组合关系 |
| `flowchart` | 算法/流程步骤 |
| `sequenceDiagram` | 跨组件交互时序 |
| `stateDiagram-v2` | 对象状态转换 |
| `graph` | 依赖关系/目录结构 |

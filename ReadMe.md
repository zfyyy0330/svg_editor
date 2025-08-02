## 整体架构图

- **正向架构图在`./imgs`文件夹**

![系统框架图](imgs/系统框架图_2.png)

### 已完成和未完成模块

- **图形绘制和编辑:**
  - [x]  {直线，矩形，圆形，五边形，六边形，星型} ，{铅笔自由线}，{文本}
  - [x]  绘制，获取，高亮显示，拖拽移动，拖拽改变形状

- **画布参数栏:**
  - [x]  高度，宽度，颜色参数
  - [x]  画布倍率缩放, 缩放时图形同步缩放
  - [x]  线宽设置, 线型设置
  - [x]  图形的填充色,图形的边框色
  - [ ] 文本的字体，字形，字大小

- **文件菜单栏**
  - [x]  新建画布
  - [x]  打开SVG，载入后二次编辑
  - [x]  保存SVG
  - [x]  导出为PNG

- **右键命令窗口/快捷键:**
  - [x]  全选，撤销
  - [x]  剪切，复制，粘贴，删除
  - [x]  置上一层，置下一层，置顶和置底
  - [x]  以上所有命令操作的redo/undo

- **配置管理:**
  - [ ]  本地配置
  - [x]  Http在线配置



## 设计亮点

#### 1. SvgChartItemBase基类设计

- 为**更好响应鼠标光标事件，如边缘判断等**，选择将`QGraphicsRectItem`作为基类，在内部提供了很多事件的默认处理方法，如笔刷和画笔的初始化，边缘判断函数，不同状态下的不同行为模式。
  - 在Base内部加入`CurrentDrawItemType` 记录**内部状态的变化**，**对外部使用透明，**方便外部获取Base指针后，直接通过**Base指针获取当前类型**，不需要外部强转去逐一判断。
  - 派生类在继承基类后，可便捷地通过设置**基类中自定义状态**来实现基类的不同行为模式，如设置当前item是否需要拖拽功能，是否需要移动功能，光标选中后是否进行边缘判断等等，对外部提供了**设置接口**，使用方便。
- **提供了很多外部可重写的虚函数**，如设置预览框的形状，当前图形检测范围等等，并提供了**默认的处理函数，外部使用方便**。
  - **核心虚函数：**
    - `virtual void setBaseRect(const QRectF& rect);`
      1. 由于将`QGraphicsRectItem`作为基类，并不足以**支撑所有自定义绘图**，如外部需要以自己的数据结构去绘制图形。因此考虑提供一个虚函数对基类的`setRect`进行封装，由派生类去自己维护，即外部**重写将自定义数据结构与基类的`boundingRect`进行绑定**，在得到新的一个QRectF时，基类调用该方法**可自动维护派生类的数据结构**。
    - `virtual SvgChartItemBase* setPreviewDict();`
      1. 在进行**拖拽和移动**时，需要预览出当前拓展对象的轮廓。因此通过**原型模式**，由子类去重写该`setPreviewDict`函数去构造自身并返回基类对象指针，以实现**多态**。

- 设计了一个通用的`SvgChartItemBaseProperty`属性以支持后续的命令操作，并提供了4个通用接口支持`SvgChartItemBase`与`SvgChartItemBaseProperty`、`QByteArray`的**序列化与反序列化**，外部使用方便。
- 为更好实现`Text`的**输入体验**，如单击即选中，双击后编辑等，提供了一个`svgChartItemTextBase`基类，去重写一些常用的事件处理函数，将其组合到`svgChartItemText`中。
  - 由于`svgChartItemText`与`svgChartItemTextBase`本质是两个Item，通过组合的方式，存在一定事件处理问题，如在鼠标点击时并没有聚焦到`svgChartItemTextBase`，`svgChartItemText`编辑和`svgChartItemTextBase`移动冲突等等。因此设计解决了在组合后2个item对于事件处理上的冲突。

#### 2. **设计规范**

- **架构清晰，分工明确，设计规范，健壮性强，耦合度低。**

  - 对于整个`mainwindow`，对于三个主操作窗口进行**隔离**，即Bar，Canvas，AttributeDisplay，继承QWidget并实现一些默认配置操作，**维护方便**。对于三个主窗口内部，对于**常用且功能覆盖率低的组件**也进行了封装，如`BarPushButton`，`PenStyleComboBox`，`ColorButton`等等，并对其内部做了基础的功能接口，以便外部使用，**较好维护且可复用**。

  - **分工明确**主要体现在**CanvasScene，CanvasView等不同类中**，即同一个权限只能交给一个类去管理和维护。
    - 将光标事件统一交给`CanvasView`进行处理，并设计了外部接口以及公开的信号，**外部信号以便外部使用**。
    - 将item有关图层顺序等命令，如置上一层，置顶等，统一交给了`CanvasScene`中，并提供了4个接口以供外部使用，**操作便捷，使用方便**。
  - **设计规范**主要体现在对于很多类的属性修改和状态编辑，**只存在一个能使它变化的接口，并做了修改验证**。其次，对于大部分接口，进行了**空指针的判断**，并且结合了单例模式，命令模式，原型模式，工厂等设计模式。
    - 如`CanvasScene`的大小和设置，只能交给`GrahpicMainCanvasWidget`去修改，如属性栏操作和Svg导入时，虽然获取到了scene，但是需要通过`emit loadSvgSceneSizeAndColorChanged`交给`GrahpicMainCanvasWidget`去修改。

  - **耦合度低**主要体现在使用了**较多的信号与槽**来进行类似于**回调的通信**，对于类与类之间的关系也尽量用**组合**的方式实现，此外也利用单例设计了全局数据类`GlobalCommonSetting`，方便**不同类中对同一参数的需求供应**，类与类之间的耦合度较低。
    - 三个主操作窗口之间的信息传递主要通过**信号与槽**进行传递。
    - 封装类中如`ColorButton`编辑完成后也通过信号的方式，由外部去自定义绑定槽来实现回调。

#### 3. **操作流畅**

## 设计不足

1. **在源码中很多`TODO：`即自我感觉设计仍然有待提升的部分。**

   1. 对于`SvgChartItemBaseProperty`的扩展性较低，**每次新增一个额外属性，都需要修改当前类，并且还需要修改当前`SvgChartItemBase`的序列化和反序列化函数，不满足开闭原则**。考虑做成一个基础类，然后有派生类去继承和重写当前`SvgChartItemBaseProperty`，并且将序列化做成一个虚函数，由外部去重写，将反序列化也作为一个虚函数，放置在`SvgChartItemBaseProperty`中支持高拓展。
   2. 对于`GrahpicAttributeDisplayWidget`中各个QWidget的初始化，**并没有将其整个QWidget进行封装，而是考虑其自由度，选择一些底层的组件进行了封装**，如`IntVaildator`和`ColorButton`，在初始化的时候直接用QLabel + 自定义组件的方式去实现，因此**存在很多冗余的初始化字段，并且较难维护**。
   3. 在修改`CanvasScene`的画布大小时，没有做到像Demo程序一样，当**设置的画布大小小于当前展示窗口大小**时，会自动等比例缩放当前scene画布，并且填充空余的背景色。
   4. 对于`GraphicAttributePenStyleComboBox`，是继承Combox以实现下拉框的选择，**存在下拉框的画笔样式图标无法沾满整个下拉框的问题**，并没有实现像Demo中整个选择框都是画笔样式图标。
   5. 对于很多类都需要`QGraphicsScene`以及`QGraphicsView`进行操作时，组合的大部分都是`QGraphicsScene`指针，而不是封装的`GraphicCanvasScene`，因此**很多类会涉及到较多`dynamic_cast`，`qgraphic_cast`的转化**，是否直接声明`GraphicCanvasScene`会更好。
   6. 对于各个enum class状态，如`DragDirection`，`CurrentBarState`等统一放在`svgGlobalConfig`，应该存放在各个相关的.h文件里更好。
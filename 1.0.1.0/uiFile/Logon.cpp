#include "Logon.h"
#include "ui_Logon.h"

Logon::Logon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logon)
{
    ui->setupUi(this);

    //初始化界面
    initLogon_Boundary();
}

Logon::~Logon()
{
    delete animation;
    delete ui;
}

//关闭界面函数
void Logon::on_clos_Btn_clicked()
{
    this->close();
}

//最小化界面函数
void Logon::on_reduce_Btn_clicked()
{
    this->showMinimized();
}

//鼠标点击事件
void Logon::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        move_Ok = true;
        this->setMouseTracking(true);   //开启鼠标追踪
        move_point = event->pos();      //获取鼠标位置
    }
}

//鼠标移动事件
void Logon::mouseMoveEvent(QMouseEvent *event)
{
    if(move_Ok){
        QPoint move_pos = event->globalPos();   //获取当前位置
        this->move(move_pos - move_point);
    }
}

//鼠标释放事件
void Logon::mouseReleaseEvent(QMouseEvent *event)
{
    move_Ok = false;
}

//登录验证
void Logon::on_logon_Btn_clicked()
{
    mySql.get_Path();
    if(testing_Input()){
        set_animation();
        /*内存泄漏*/
        UserInfo *UserInfo_Widget = new UserInfo();
        UserInfo_Widget->show();

        this->close();
    }
}

//注册账号
void Logon::on_register_Btn_clicked()
{
    Register* Register_GUI = new Register(nullptr,this);
    Register_GUI->show();
}

//绘制动画
void Logon::set_animation()
{
    //设置控件不可见
    ui->usernumber_Box->hide();
    ui->password_Edit->hide();
    ui->checkBox->hide();
    ui->checkBox_2->hide();
    ui->register_Btn_2->hide();
    ui->logon_Btn->hide();
    ui->register_Btn->hide();
    animation->start();//执行GUI动画
}

//设置
void Logon::on_setup_Btn_clicked()
{
    Setup* Setup_GUI = new Setup();
    Setup_GUI->show();
}

//验证窗口
bool Logon::testing_Input()
{
    //获取输入框内容
    input_Usernumber = ui->usernumber_Box->currentText();
    input_Password = ui->password_Edit->text();

    int pos = 0;
    if(QValidator::Acceptable != usernumber_Validator->validate(input_Usernumber,pos) ||
            !mySql.get_SqlQuery("user", "number",input_Usernumber)){
        //验证号码是否存在且正确
        QMessageBox::information(nullptr,"提示","账号错误或未注册!",QMessageBox::Ok);
        return false;
    }
    else if(QValidator::Acceptable != password_Validator->validate(input_Password,pos) ||
            !mySql.get_SqlQuery("user", "password",input_Password)){
        //验证密码是否正确
        QMessageBox::information(nullptr,"提示","密码错误！",QMessageBox::Ok);
        return false;
    }
    else{
        return true;
    }
}

//初始化界面
bool Logon::initLogon_Boundary()
{
    //初始化参数
    move_Ok = false;
    input_Usernumber = "";
    input_Password = "";

    //去掉边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);


    //设置Top中的gif动画
    QMovie *top_Gif = new QMovie(this);
    top_Gif->setFileName(":/new/logo/icons/logo/Top.gif");
    top_Gif->start();
    ui->Top->setMovie(top_Gif);

    //设置username_Box提示
    ui->usernumber_Box->lineEdit()->setPlaceholderText("QQ号码/手机/邮箱");

    //设置user_image为圆形
    QPixmap pixmap(":/new/user/user/My.jpg");
    pixmap = pixmap.scaled(ui->user_image->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap roundPixmap(ui->user_image->size());
    roundPixmap.fill(Qt::transparent);
    QPainter painter(&roundPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawEllipse(roundPixmap.rect());
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(0, 0, pixmap);
    ui->user_image->setPixmap(roundPixmap);

    //设置登录动画
    animation=new QPropertyAnimation(ui->user_image, "geometry");     //设置user_image的位置大小
    animation->setDuration(400);                        //设置动画完成双击为400毫秒
    animation->setStartValue(QRect(160, 90, 85, 85));  //设置动画初始位置值
    animation->setEndValue(QRect(160, 175, 85, 85));   //设置动画结束位置值

    //初始化输入验证器，限制输入为数值
    usernumber_Validator = new QRegularExpressionValidator(QRegularExpression("\\d{10}"), this);
    password_Validator = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9].{10,14}"), this);
    ui->usernumber_Box->setValidator(usernumber_Validator);
    ui->password_Edit->setValidator(password_Validator);
}


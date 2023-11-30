### 【实战项目】麦轮遥控小车+机械臂

1. PS2遥控模块（SPI）
2. 电机驱动模块（麦轮运动）
3. 舵机驱动模块（简易机械臂）

。。。

---

### git使用：

#### 编写代码后推送：

1. `git add .`
2. `git commit -m "修改XX代码"`
3. `git push`

#### 第一次本地项目上传：

1. gitee先创建仓库（名字、地址、其他默认）
2. `git init`
3. `touch README.md` 并编辑
4. `git add .`
5. `git commit -m "第一次提交"`
6. `git remote add origin https://gitee.com/fleeting-years-ljjj/McWheel-Remote-Car-with-Robotic-Arm.git`
7. `git push -u origin "master"`



---

**关于 git push -u origin "master" 与 git push origin master**

这两个命令的区别在于第一个命令使用了**"-u"标志来设置上游分支**，而第二个命令则假设上游分支已经设置好。

当你第一次使用"git push -u origin master"命令时，它会将本地的"master"分支推送到名为"origin"的远程仓库，并将本地的"master"分支与远程的"master"分支建立关联。这样，在以后的推送操作中，**你只需要使用"git push"命令**，Git就会自动将本地的"master"分支推送到远程的"master"分支。

而当你已经通过第一个命令建立了本地与远程分支的关联后，以后的推送操作可以直接使用"git push origin master"命令。这个命令的作用与第一个命令相同，都是将本地的"master"分支推送到远程的"master"分支。

因此，第一个命令主要用于首次建立本地与远程分支的关联，而第二个命令则是在关联已经建立好的情况下进行推送操作。

**如果第一次就用git push origin master呢？**

如果在第一次推送时使用"git push origin master"命令而不使用"-u"标志来设置上游分支，Git会将本地的"master"分支推送到远程的"master"分支，但不会建立本地与远程分支的关联。

这意味着，以后的推送操作需要手动指定远程分支，即**每次都需要使用"git push origin master"命令**来推送本地的"master"分支到远程的"master"分支。

相比之下，使用"git push -u origin master"命令可以在第一次推送时建立本地与远程分支的关联，以后的推送操作就可以简化为**只需使用"git push"命令**即可。

所以，如果你想在第一次推送时就建立本地与远程分支的关联，**推荐使用"git push -u origin master"命令**。


直线与圆求交

## 性能加速

在程序中使用了openmp加速，如并行多行渲染：

![](/Users/toby/Documents/Cources/08-图形学/project/results/RayTracing-openmp.bmp)

## 算法加速



## 贝塞尔曲线求交

由于本身希望做一个类似于沙漏、水杯这类东西，这是一个一维贝塞尔曲线绕指定轴旋转而成的，我们需要得到光线与曲面的交点信息。

四维贝塞尔曲线$P(u)$通过4个控制点定义，坐落于2维平面上

$$
P(u) = \left[ \matrix{P_x(u) \\ P_y(u)} \right] = 
\left[ 
	\matrix{
		\sum _{i=0}^n P_i^{(x)} (1-u)^{n-i}u^i \\
		\sum _{i=0}^n P_i^{(y)} (1-u)^{n-i}u^i
	}
 \right]
$$

$$
\frac{dP}{du} = n \sum_{i=0}^{n-1} P_i (B_{i-1,n-1}(u) - B_{i,n-2}(u))
$$

设光线为$C(t)$，曲面为$S(u,\theta)$

$$
C(t) = \left[ 
	\matrix{
		O_x + t D_x \\
		O_y + t D_y \\
		O_z + t D_z \\
		}
\right]
$$

$$
S(u,\theta) = \left[
	\matrix{
		Q_x + sin\theta P_x(u) \\
		Q_y + cos\theta P_x(u) \\
		Q_z + P_y(u)
	}
\right]
$$

分别对于$C(t)$,$S(u,\theta)$求导

$$
\frac{dC}{dt} = \left[ \matrix{D_x \\ D_y \\ D_z}\right]
$$

$$
\frac{\partial S}{\partial u} = \left[ 
	\matrix{
		sin\theta \frac{d{P_x}}{d{u}} \\
		cos\theta \frac{d{P_x}}{d{u}} \\
		\frac{\partial{P_y}}{\partial{u}}
	}
	\right]
$$
$$
\frac{\partial S}{\partial \theta} = \left[ 
	\matrix{
		cos\theta P_x(u) \\
		-sin\theta P_x(u) \\
		0
	}
	\right]
$$

最小化$F(u,\theta,t) = C(t) - S(u,\theta)$

显然
$$
\frac{\partial F}{\partial t} =  \left[ 
\matrix{
\frac{\partial F_1}{\partial t} & \frac{\partial F_1}{\partial u} & \frac{\partial F_1}{\partial \theta} \\
\frac{\partial F_2}{\partial t} & \frac{\partial F_2}{\partial u} & \frac{\partial F_2}{\partial \theta} \\
\frac{\partial F_3}{\partial t} & \frac{\partial F_3}{\partial u} & \frac{\partial F_3}{\partial \theta} \\
} \right] 
$$

$$
\frac{\partial F}{\partial t} = \left[
\matrix{
	D_x & sin\theta \frac{dP_x}{du} & cos\theta P_x(u)\\
	D_y & cos\theta \frac{dP_x}{du} & -sin\theta P_x(u)\\
	D_z & \frac{dP_y}{du} & 0\\
}
\right]
$$
其中

$$
\frac{dP}{du} = n \sum_{i=0}^{n-1} P_i (B_{i-1,n-1}(u) - B_{i,n-2}(u))
$$

将通过牛顿迭代，我们可以计算出F函数的零点，其中有两点需要注意的地方

* 牛顿迭代需要保证答案中$u \in [0,1]$，而牛顿迭代本身无法附加条件，因此需要在答案的邻域中查找合适的$u$,$t$,$\theta$，我用到的方法是，对于给定曲线，生成一个圆柱体包含框，随机该圆柱体里面高度$h$的一个圆形面片，将光线与该面片的交点的$u$,$t$,$\theta$作为迭代的初值。随机$h$约30次即可得解。

* 由于牛顿迭代不太精确，可能是的结果产生微小偏移，这种偏移会对判断点在平面哪一侧产生影响，我们可以通过调整eps的取值，不同部分赋予不同的eps，使得这些误差不至于相互影响。
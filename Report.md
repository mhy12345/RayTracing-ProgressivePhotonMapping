直线与圆求交



贝塞尔曲线求交

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
library(tidyverse)

# Parameters --------------------------------------------------------------
# Initial conditions

R1.init <- 0.899
R2.init <- 0.01
p.init <- 0
s.init <- 0
v1.init <- 0
v2.init <- 0

# Parameters
kappa1 <- 1
kappa2 <- 1

alpha <- 1
beta <- 1
omega <- 1

gamma1 <- 0.54
gamma2 <- gamma1

sigma1 <- 0.1
sigma2 <- 0.1

epsilon1 <- 0.1
epsilon2 <- 0.1

delta1 <- 0.1
delta2 <- 0.1

# Number of interations
options(scipen = 999)
n.iter <- 15000

# Flags
# fixed.point.select <- "third.plus"

params <- c(n.iter,
						R1.init, R2.init, p.init, s.init,
						v1.init, v2.init,
						kappa1, kappa2, alpha, beta, gamma1, gamma2, sigma1, sigma2,
						epsilon1, epsilon2, delta1, delta2)

#Compile
system("gcc -Ofast -lm 6eq_rk4_bipartite_test.c -o compiled/rk4-test")
system(paste("./compiled/rk4-test", paste(params, collapse = ' '), "> data/results-test.csv"))

# Read data
data <- data.table::fread("data/results-test.csv")


# saveRDS(data, "data/6eq.det.R1080.R21.gamma02.rds")

# Scale
scale <- 1000

# Create plot
gg <- ggplot(data %>% mutate (V2 = V2 * scale, V3 = V3 * scale, V4 = V4 * scale, V5 = V5 * scale, V6 = V6 * scale, V7 = V7 * scale)) +
geom_line(aes(x = V1, y = V2, colour = "R1")) +
geom_line(aes(x = V1, y = V3, colour = "R2")) +
geom_line(aes(x = V1, y = V4, colour = "p")) +
geom_line(aes(x = V1, y = V5, colour = "s")) +
geom_line(aes(x = V1, y = V6, colour = "v1")) +
geom_line(aes(x = V1, y = V7, colour = "v2")) +
labs(x = "Time", y = "Variables", color = "Variable") +
scale_color_manual(values = c("R1" = "red3", "R2" = "royalblue",
																"p" = "springgreen4", "s" = "purple3",
																"v1" = "black", "v2" = "orange"),
										 breaks = c("R1", "R2", "p", "s", "v1", "v2"),
										 labels = c("R1", "R2", "p", "s", "v1", "v2")) +
theme_bw()

gg

saveRDS(gg, paste0("objects/", "gg.6eq.det.R1800.R21000.gamma02.rds"))


# tail(data)


# # Second Fixed Point
# a <- kappa1 * (sigma1/omega + 1)
# b <- -(kappa1 + gamma1)
# c <- gamma1
#
# p.star2.plus <- (-b + sqrt(b^2 - 4*a*c)) / (2*a)
# R1.star2.plus <- 1 - gamma1/(kappa1 * p.star2.plus)
#
# p.star2.minus <- (-b - sqrt(b^2 - 4*a*c)) / (2*a)
# R1.star2.minus <- 1 - gamma1/(kappa1 * p.star2.minus)

# # Third Fixed Point
# s.star3 <- tail(data$V5,1)
#
# a3 <- (sigma1 / omega + 1 ) * kappa1
# b3 <- (sigma1 / omega) * kappa1 * s.star3 - kappa1 * (1 - s.star3) - epsilon1 * s.star3 - gamma1
# c3 <- (epsilon1*s.star3 + gamma1) * (1 - s.star3)
#
# p.star3.p <- (-b3 + sqrt(b3^2 - 4*a3*c3)) / (2*a3)
# R1.star3.p <- (kappa1*p.star3.p - epsilon1*s.star3 - gamma1) / (kappa1 * (p.star3.p + s.star3))
# R2.star3.p <- R1.star3.p * s.star3 / p.star3.p
# v1.star3.p <- epsilon1*R1.star3.p*s.star3 / delta1
# v2.star3.p <- epsilon2*R2.star3.p*s.star3 / delta2
#
# p.star3.m <- (-b3 - sqrt(b3^2 - 4*a3*c3)) / (2*a3)
# R1.star3.m <- (kappa1*p.star3.m - epsilon1*s.star3 - gamma1) / (kappa1 * (p.star3.m + s.star3))
# R2.star3.m <- R1.star3.m * p.star3.m / s.star3
# v1.star3.m <- epsilon1*R1.star3.m*s.star3 / delta1
# v2.star3.m <- epsilon2*R2.star3.m*s.star3 / delta2
#
#
# s.star3 <- s.star3 * scale
# p.star3.p <- p.star3.p * scale
# R1.star3.p <-R1.star3.p * scale
# R2.star3.p <- R2.star3.p * scale
# v1.star3.p <- v1.star3.p * scale
# v2.star3.p <- v2.star3.p * scale
# p.star3.m <- p.star3.m * scale
# R1.star3.m <- R1.star3.m * scale
# R2.star3.m <- R2.star3.m * scale
# v1.star3.m <- v1.star3.m * scale
# v2.star3.m <- v2.star3.m * scale
#
#
#
#
# if (fixed.point.select == "second.plus") {
# 	gg +
# 		geom_hline( yintercept = p.star2.plus, color = "springgreen4", linetype="dotted" ) +
# 		geom_hline( yintercept = R1.star2.plus, color = "red3", linetype="dotted" )
# } else if (fixed.point.select == "second.minus") {
# 	gg +
# 		geom_hline( yintercept = p.star2.minus, color = "springgreen4", linetype="dotted" ) +
# 		geom_hline( yintercept = R1.star2.minus, color = "red3", linetype="dotted" )
# }else if (fixed.point.select == "third.plus")  {
# 	gg +
# 		geom_hline( yintercept = p.star3.p, color = "springgreen4", linetype="dotted" ) +
# 		geom_hline( yintercept = R1.star3.p, color = "red3", linetype="dotted" ) +
# 		geom_hline( yintercept = R2.star3.p, color = "royalblue", linetype="dotted" ) +
# 		geom_hline( yintercept = s.star3, color = "purple3", linetype="dotted" ) +
# 		geom_hline( yintercept = v1.star3.p, color = "black", linetype="dotted" ) +
# 		geom_hline( yintercept = v2.star3.p, color = "orange", linetype="dotted" )
# }else if (fixed.point.select == "third.minus")  {
# 	gg +
# 		geom_hline( yintercept = p.star3.m, color = "springgreen4", linetype="dotted" ) +
# 		geom_hline( yintercept = R1.star3.m, color = "red3", linetype="dotted" ) +
# 		geom_hline( yintercept = R2.star3.m, color = "royalblue", linetype="dotted" ) +
# 		geom_hline( yintercept = s.star3, color = "purple3", linetype="dotted" ) +
# 		geom_hline( yintercept = v1.star3.m, color = "black", linetype="dotted" ) +
# 		geom_hline( yintercept = v2.star3.m, color = "orange", linetype="dotted" )
# }





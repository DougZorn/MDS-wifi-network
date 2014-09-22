rawRead = read.csv("testnode4.csv", header = F)
n = dim(rawRead)[1]
distance = matrix(0, nrow=n, ncol=n)
delta = matrix(0, nrow=n, ncol=n)
coA = 1.518
coB = 0.270
StandarDev = 0.02798/sqrt(2)

Y = matrix(c(0, -2, 0, 0, 1, 0, -1, 1, 1, 1, -1, -1), nrow=6, ncol=2, byrow=T)
rawRead <- .5*(rawRead + t(rawRead)) 
for(v in 1:n){
	for(w in 1:n){
		rawRead[v,w] <- if(rawRead[v,w]== 0) 0 else if(rawRead[v,w]>=128) ((rawRead[v,w]-256)/2-71) else (rawRead[v,w]/2-71)
	}
}
for(v in 1:n){
	for(w in 1:n){	
		delta[v,w] <- if(rawRead[v,w]== 0) 0 else (log(10)/coB)*10^(((log10(-rawRead[v,w])-coA)/coB))*StandarDev
		distance[v,w] <- if(rawRead[v,w]== 0) 0 else 10^((log10(-rawRead[v,w])-coA)/coB)
	}
}

dstar = matrix(0, nrow=n, ncol=n)

rrr    = 1000
Z = array(0, dim=c(rrr, n, 2)) 
for(k in 1:rrr){
  for(i in 1:(n-1)){ 
    for(j in (i+1):n){
	  dstar[i,j] = dstar[j,i] = rnorm(1, distance[i,j], delta[i,j]) #### symmetrical rnorm fill, even though delta is symmetrical when running rnorm it needs to be symmetrical as well
	}
  }
  fit<-cmdscale(dstar, eig = TRUE, k = 2)
  X = fit$points
  b = c(0, -2, 0, 0, 1, 0)
  D = matrix(c(1, 0, X[1,1], X[1,2], 0, 0, 
               0, 1, 0, 0, X[1,1], X[1,2],
               1, 0, X[2,1], X[2,2], 0, 0,
               0, 1, 0, 0, X[2,1], X[2,2],
               1, 0, X[3,1], X[3,2], 0, 0,
               0, 1, 0, 0, X[3,1], X[3,2]), nrow=6, ncol=6, byrow=T)
  cc = solve(D,b)
  mu = c(cc[1], cc[2])
  A = matrix( c(cc[3], cc[4], cc[5], cc[6]), nrow=2, ncol=2, byrow=T)
  
  for(i in 1: n){   ### Z is dstar? rebasing/mapping to new coordinate system
    Z[k,i,] = mu + A%*%X[i,]
  }
}

fitRecon<-cmdscale(distance, eig = TRUE, k = 2)
L = fitRecon$points
bb = c(0, -2, 0, 0, 1, 0)
DD = matrix(c(1, 0, L[1,1], L[1,2], 0, 0, 
            0, 1, 0, 0, L[1,1], L[1,2],
            1, 0, L[2,1], L[2,2], 0, 0,
            0, 1, 0, 0, L[2,1], L[2,2],
            1, 0, L[3,1], L[3,2], 0, 0,
            0, 1, 0, 0, L[3,1], L[3,2]), nrow=6, ncol=6, byrow=T)
CC = solve(DD,bb)
mumu = c(CC[1], CC[2])
AA = matrix( c(CC[3], CC[4], CC[5], CC[6]), nrow=2, ncol=2, byrow=T)
P = L
for(i in 1: n){
  P[i,] = mumu + AA%*%L[i,]
}

z4star = Z[,4,]

x11()
par(mar=c(4,4,1,1)+0.1)
plot(Z[,,1], Z[,,2], xlab="feet", ylab="feet", main="6 node Map", type="n",xlim=c(-7,7),ylim=c(-7,7))
points(z4star[,1],z4star[,2], col=2, pch=20)
points(P[,1],P[,2], col=4, pch=17, cex=1.9)
points(Y[1:n,1],Y[1:n,2], col=1, pch=19, cex=1.5)
text(Y[1:n,1],Y[1:n,2],labels = row(fit$points), pos=3, cex=1.25)
text(P[,1],P[,2],labels = row(fitRecon$points), pos=1, col=4,cex=.75)
legend(4,6,legend=c("Truth", "Reconst", "Node 4 distrib"), pch=c(19,17,20), col=c(1,4,2), bty="n")


x11()
par(mar=c(4,4,1,1)+0.1)
plot(Z[,1], Z[,2], xlab="feet", ylab="feet", main="Map", type="n",xlim=c(-3,3),ylim=c(-3,3))
points(Y[1:n,1],Y[1:n,2], col=1, pch=19, cex=1.5)
text(Y[1:n,1],Y[1:n,2],labels = row(fit$points), pos=3, cex=1.25)
points(Z[,1],Z[,2], col=2, pch=20)
text(Z[,1],Z[,2],labels = row(fit$points), pos=3, col=2)
legend(4,4,legend=c("Truth", "Reconst"), pch=c(19,20), col=c(1,2), bty="n")






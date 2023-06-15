[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Medical-CovidR0#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/signaloid/Signaloid-Demo-Medical-CovidR0#gh-light-mode-only)

<br/>
<br/>

# Calculating R0 for COVID-19
The R0 number is a way of rating coronavirus or any disease's ability to spread. This application calculates the R0 for COVID-19, based on a model by Mwalili et. al.[^0].

<br/>
<br/>
<br/>

## What is R0 and why is it useful?
In epidimiology, the _basic reproduction number_ or R0 of an infection is a metric to assess the spread of a virus. R0 is the number of people that one infected person will pass on a virus to, on average. For example, the R0 of measles is 15 in populations without immunity. During the COVID-19 pandemic, R0 was an important indicator for governments and health officials to determine measures of fighting the virus as well as plan their strategy. A significant attribute of R0 is the estimate of if it has value greater or less than 1, because R0 < 1 indicates that the virus spread is diminishing.

## Calculation model
For this application example, the model for R0 takes into account many metrics such as population birth rate, pathogen's death rate, metrics that model transmission from different population groups, etc. In our implementation we use default values found in the article[^0] and we assume that each of those values has a standard deviation from its mean value and, as a result, we modelled them as normal distributions. For the birth rate of the population, we use real life data for the life expectancy of people in Greece and United Kingdom. Finally, for some metrics we use the mean values only without adding distributional information. One can easily make the result more account by modifying the original program to input from real world data for the other metrics. The source code of the program can be found [here](src/main.c).

```c
/*
 * Distributions from real world data
 */

lifeExpectancy = UxHwDoubleDistFromSamples(lifeExpectancyData, kMaxData);
mu = 1 / lifeExpectancy;

/*
 * Adding distributional information
 */
mp = UxHwDoubleGaussDist(pathogenDeathRateMean, pathogenDeathRateDeviation);

/*
 * Keeping just the mean value
 */
ga =  gaMeanValue;

```

## Running the program

You can run the program yourself by clicking on the "Add to signaloid.io" button at the top of the page. Then, on the signaloid.io page you can just build and run this application. You can also try using the `greece-life-expectancy.in`[^1] or the `united-kingdom-life-expectancy.in`[^2] as the first argument of the program to load the data for human life expectancy. You can also modify the `kMaxData` constant that defines the amount of samples that are going to be read from the input file. 

## Output of the program

Running the program with the default values and get the following result.

<img src="https://user-images.githubusercontent.com/37593410/170267573-efabfe7f-b467-4112-9a81-b44bbabe9187.png" width="50%"/>

If we run the program using data for life expectancy in Greece, we get the following result.

<img src="https://user-images.githubusercontent.com/37593410/170267977-fe5a5a6a-048e-4f75-97a9-e7482113d258.png" width="50%"/>

Finally, running the program using data for life expectancy in UK, we get the following result.

<img src="https://user-images.githubusercontent.com/37593410/170268109-5ad84ee6-6650-4787-9883-6016a11e5093.png" width="50%"/>

In the calculation of R0, it is important for the authorities to know if it has crossed the threshold of 1. A mean value may not have crossed this threshold but depending on the distributional information, health officials can know the possibility of this value exceeding 1, and, in general, have a better understanding of how the pandemic evolves over time.

Combining the Uncertainty Tracking Technology as well as real world and population-specific data we can have a better estimation of the R0 and the development of the pandemic, that would allow health officials and authorities to take better-informed decisions.


<br/>
<br/>
<br/>


[^0]: Mwalili, S., Kimathi, M., Ojiambo, V., Gathungu, D., & Mbogo, R. (2020). SEIR model for COVID-19 dynamics incorporating the environment and social distancing. BMC research notes, 13(1), 352. https://doi.org/10.1186/s13104-020-05192-1
[^1]: <a href='https://www.macrotrends.net/countries/GRC/greece/life-expectancy'>Greece Life Expectancy 1950-2022</a>. www.macrotrends.net. Retrieved 2022-05-24.
[^2]: <a href='https://www.macrotrends.net/countries/GBR/united-kingdom/life-expectancy'>U.K. Life Expectancy 1950-2022</a>. www.macrotrends.net. Retrieved 2022-05-24.

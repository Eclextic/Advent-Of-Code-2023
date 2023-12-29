use std::{env, ffi::OsString, io::{BufReader, self, BufRead}, fs::File};

fn read_file(path: &str) -> io::Result<Vec<String>> {
    let file = File::open(path)?;
    let reader = BufReader::new(file);

    let mut lines: Vec<String> = Vec::new();
    for line in reader.lines() {
        lines.push(line?)
    }

    Ok(lines)
}

fn read_arg() -> Vec<String> {
    let args = env::args_os().collect::<Vec<OsString>>();
    let path = args[1].to_str().unwrap();

    if let Ok(vec) = read_file(path) {
        vec
    } else {
        println!("Path specified does not exist!");
        std::process::exit(0);
    }
}

fn distance(wait_time: i32, max_wait: i32) -> i32 {
    (max_wait - wait_time) * wait_time
}

fn possible_ways(min_held_time: i32, max_held_time: i32) -> i32 {
    max_held_time - min_held_time + 1
}

#[derive(Debug)]
struct Race { time: i32, dist: i32 }

fn main() {
    let file = read_arg();
    let times: Vec<_> = file[0].split_whitespace().skip(1).collect();
    let distances: Vec<_> = file[1].split_whitespace().skip(1).collect();

    let races: Vec<_> = times.iter().zip(distances.iter()).map(|x| Race { time: x.0.parse().unwrap(), dist: x.1.parse().unwrap()}).collect();

    let mut possibles = vec![0; races.len()];

    let mut x = 0;
    for race in &races {
        let mut min: i32 = 0;
        let mut min_dist = 0;
        for i in 1..race.time {
            min_dist = distance(i, race.time);
            if min_dist > race.dist {
                min = i;
                break;
            }
        }

        let mut max: i32 = 0;
        let mut max_dist = 0;
        for i in (1..race.time).rev() {
            max_dist = distance(i, race.time);
            if max_dist > race.dist {
                max = i;
                break;
            }
        }
        // let mut max: i32 = 0;
        // let mut max_dist = 0;
        // for i in (1..race.time).rev() {
        //     let new_dist = distance(i, race.time);
        //     if max_dist < new_dist {
        //         max_dist = new_dist;
        //     } else if max_dist > new_dist {
        //         max = i;
        //         break;
        //     }
        // }

        possibles[x] = possible_ways(min, max);
        println!("Race Number: {:?}, Minimum Distance: {:?}, Maximum Distance: {:?}, Min: {:?}, Max: {:?}", x, min_dist, max_dist, min, max);
        println!("Possible ways of solving: {}", possibles[x]);
        println!();

        x += 1;
    }

    println!("Product of possible ways to solve: {}", possibles.iter().product::<i32>());
}

// d = (tmax - twait) * twait
// P = tmaxheld - tminheld - 1

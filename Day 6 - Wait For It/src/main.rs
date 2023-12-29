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

fn distance(wait_time: i64, max_wait: i64) -> i64 {
    (max_wait - wait_time) * wait_time
}

fn possible_ways(min_held_time: i64, max_held_time: i64) -> i64 {
    max_held_time - min_held_time + 1
}

#[derive(Debug)]
struct Race { time: i64, dist: i64 }

fn main() {
    let file = read_arg();
    let time = file[0].strip_prefix("Time: ").unwrap().replace(" ", "");
    let dist = file[1].strip_prefix("Distance: ").unwrap().replace(" ", "");

    let race = Race { time: time.parse().unwrap(), dist: dist.parse().unwrap() };

    let mut min: i64 = 0;
    let mut min_dist = 0;
    for i in 1..race.time {
        min_dist = distance(i, race.time);
        if min_dist > race.dist {
            min = i;
            break;
        }
    }

    let mut max: i64 = 0;
    let mut max_dist = 0;
    for i in (1..race.time).rev() {
        max_dist = distance(i, race.time);
        if max_dist > race.dist {
            max = i;
            break;
        }
    }

    println!("Minimum Distance: {:?}, Maximum Distance: {:?}, Min: {:?}, Max: {:?}", min_dist, max_dist, min, max);
    println!("Possible ways of solving: {}", possible_ways(min, max));
}

// d = (tmax - twait) * twait
// P = tmaxheld - tminheld - 1

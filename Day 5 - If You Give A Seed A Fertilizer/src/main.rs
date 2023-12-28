use std::{env::{self}, io::{BufReader, BufRead, self}, fs::File, ffi::OsString, ops::Range};

#[derive(Debug)]
struct ConversionMap<'a> {
    from: &'a str,
    to: &'a str,
    values: Vec<Converter>
}

#[derive(Debug)]
struct Converter {
    conversion_range: Range<i64>,
    difference: i64,
}

impl Convert for Converter {
    fn convert(self: &Self, value: &mut i64) {
        *value += self.difference;
    }
}

trait Convert {
    fn convert(self: &Self, value: &mut i64);
}

trait InRange {
    fn is_in_range(self: &Self, val: i64) -> bool;
}

impl InRange for Range<i64> {
    fn is_in_range(self: &Self, val: i64) -> bool {
        self.start <= val && val < self.end
    }
}

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

fn create_converter(str: &String) -> Converter {
    let values: Vec<_> = str.split(" ").collect();
    let range_start = values[1].parse::<i64>().unwrap();
    let range_len = values[2].parse::<i64>().unwrap();

    let difference = (values[0].parse::<i64>().unwrap()) - range_start;

    Converter { conversion_range: range_start..range_start+range_len, difference }
}

fn create_conversion_map(str_slice: &[String]) -> ConversionMap {
    let from_to_str = str_slice[0].strip_suffix(" map:").unwrap().split("-to-").collect::<Vec<_>>();
    let values: Vec<_> = str_slice[1..].iter().map(|x| create_converter(x)).collect();

    ConversionMap { from: from_to_str[0], to: from_to_str[1], values}
}

fn main() {
    let file = read_arg();
    let lines: Vec<&[String]> = file.split(|str| str == "").collect();
    let seeds: Vec<i64> = lines[0][0].strip_prefix("seeds: ").expect("No prefix?").split(" ").map(|x| x.parse::<i64>().unwrap()).collect();

    let conversion_maps: Vec<_> = lines[1..].iter().map(|x| create_conversion_map(x)).collect();

    let mut locations: Vec<i64> = Vec::new();
    for seed in &seeds {
        let mut aggregate: i64 = *seed;
        for map in &conversion_maps {
            if let Some(converter) = map.values.iter().find(|x| x.conversion_range.is_in_range(aggregate)) {
                converter.convert(&mut aggregate);
            }
        }

        locations.push(aggregate);
    }

    println!("{:?}", locations.iter().min());
}

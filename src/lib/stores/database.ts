import type { Database } from "firebase/database"
import { writable } from "svelte/store"

export const db = writable<Database>()